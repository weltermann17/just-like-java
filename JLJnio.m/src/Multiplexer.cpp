// Multiplexer.cpp

//********************************************************************

#ifndef Multiplexer_cpp
#define Multiplexer_cpp

//********************************************************************

#include <jlj/nio/reactor/Multiplexer.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/ConnectionLifetimeManager.h>
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
using namespace jlj::util::concurrent;
#include <jlj/util/concurrent/locks/ReentrantLock.h>
using namespace jlj::util::concurrent::locks;
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/StreamAnything.h>
#include <jlj/util/Uuid.h>
using namespace jlj::util;
#include <jlj/util/zip/ZipInputStream.h>
#include <jlj/util/zip/ZipOutputStream.h>
using namespace jlj::util::zip;
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;
#include <jlj/lang/Thread.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

MultiplexerI::~MultiplexerI() throw (IOException)
{
	try
	{
		ChunkBufferPool::getInstance()->makeIdle(destination);
		ChunkBufferPool::getInstance()->makeIdle(incomplete);
	}		
	catch (const Exception& e)
	{
		Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::SEVERE, L"MultiplexerI::~MultiplexerI() : " + e->toString());
	}
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::INFO, L"MultiplexerI::~MultiplexerI()");
}

//********************************************************************

MultiplexerI::MultiplexerI(const String& id)
						   : id(id)
						   , unavailable(0)
						   , state(STATE_INITIAL)
						   , incomplete(ChunkBufferPool::getInstance()->get())
						   , destination(0)
{}

//********************************************************************

void MultiplexerI::readFully(ChunkBuffer& dst,
							 ChunkBuffer& src,
							 int l,
							 const String& id,
							 const String& streamid,
							 int& avail) 
{
	if (0 >= l) return;

	if (0 == dst)
	{
		dst = ChunkBufferPool::getInstance()->get();
	} 
	else if (l > dst->limit)
	{
		l -= dst->limit;
		if (0 < dst->limit) 
		{
			readFully(dst, src, dst->limit, id, streamid, avail); 
		}

		ConnectionLifetimeManager::getInstance(id)->getChunkedInputStream(streamid)->pipe(dst);
		dst = ChunkBufferPool::getInstance()->get();
	}
	memcpy(dst->buffer + dst->position, src->buffer + src->position, l); 
	src->position += l; 
	src->limit -= l; 
	dst->position += l; 
	dst->limit -= l; 

	avail -= l;
}

//********************************************************************

void MultiplexerI::parseBuffer(InputStream socketinputstream) throw (IOException)
{
#define readfully(dst, src, len) \
	readFully(dst, src, len, id, streamid, available); 

#define resetincomplete() \
	ChunkBufferPool::getInstance()->clearBuffer(incomplete); 

	ChunkBuffer source = ChunkBufferPool::getInstance()->get();

	try
	{
		ConnectionLifetimeManager lifetimemanager = ConnectionLifetimeManager::getInstance(id);
		
		int available = socketinputstream->available();

		source->limit = socketinputstream->read(source->buffer, 0, available);

		int command = 0;
		int required = 0;

		bool ready = false;

		while (!ready)
		{
			switch (state)
			{
			case STATE_INITIAL:
			case STATE_CONNECTION:
				{
					required = 0 < unavailable ? unavailable : Multiplexer::IDENTIFIERLENGTH;
					if (required > available)
					{
						state = STATE_INCOMPLETEREAD;
						nextstate = STATE_CONNECTION;
					}
					else
					{
						state = STATE_COMPLETEREAD;
						nextstate = STATE_COMPLETECONNECTION;
					}
					break;
				}
			case STATE_INCOMPLETEREAD:
				{
					int n = available;
					readfully(incomplete, source, available);
					unavailable = required - n;
					state = nextstate;
					ready = true;
					break;
				}
			case STATE_COMPLETEREAD:
				{
					readfully(incomplete, source, required);
					unavailable = 0;
					state = nextstate;
					break;
				}
			case STATE_COMPLETECONNECTION:
				{
					String oldid = streamid;
					streamid = String(incomplete->buffer, Multiplexer::IDENTIFIERLENGTH);
					resetincomplete();

					if (ConnectionI::NULLSTREAMID == streamid) fireEvent(NCR);

					bool connectionisdifferent = 0 < oldid->length() && streamid != oldid;
					bool needtoflushbuffer = 0 != destination && 0 < destination->position;

					if (connectionisdifferent && needtoflushbuffer)
					{
						lifetimemanager->getChunkedInputStream(oldid)->pipe(destination);
						destination = ChunkBufferPool::getInstance()->get();
					}

					state = STATE_COMMAND;
					break;
				}
			case STATE_COMMAND:
				{
					required = 0 < unavailable ? unavailable : sizeof(int);
					if (required > available)
					{
						state = STATE_INCOMPLETEREAD;
						nextstate = STATE_COMMAND;
					}
					else
					{
						state = STATE_COMPLETEREAD;
						nextstate = STATE_COMPLETECOMMAND;
					}
					break;
				}
			case STATE_COMPLETECOMMAND:
				{
					memcpy((char*) &command, incomplete->buffer, sizeof(int));
					resetincomplete();

					command = bigendian(command);

					if (Multiplexer::ENDOFBUFFER == command)
					{
						state = STATE_ENDOFBUFFER;
					}
					else if (Multiplexer::ENDOFFILE == command)
					{
						state = STATE_ENDOFFILE;
					}
					else
					{
						if (0 < command && command <= ChunkBufferPoolI::DEFAULTCAPACITY)
						{
							state = STATE_BUFFER;
						}
						else
						{
							state = STATE_INVALID;
						}
						break;
					}
					break;
				}
			case STATE_ENDOFBUFFER:
				{
					state = STATE_FINAL;
					break;
				}
			case STATE_ENDOFFILE:
				{
					ChunkedInputStream chunkedinputstream = lifetimemanager->getChunkedInputStream(streamid);
					if (0 != destination) 
					{
						chunkedinputstream->pipe(destination);
						destination = 0;
					}
					chunkedinputstream->pipe(0);
					state = STATE_COMMAND;
					break;
				}
			case STATE_BUFFER:
				{
					required = 0 < command ? command : unavailable;
					if (required > available)
					{
						state = STATE_INCOMPLETEBUFFERREAD;
					}
					else
					{
						state = STATE_COMPLETEBUFFER;
					}
					break;
				}
			case STATE_INCOMPLETEBUFFERREAD:
				{
					int n = available;
					readfully(destination, source, available);
					unavailable = required - n;
					state = STATE_BUFFER;
					ready = true;
					break;
				}
			case STATE_COMPLETEBUFFER:
				{
					readfully(destination, source, required);
					unavailable = 0;
					state = STATE_COMMAND;
					break;
				}
			case STATE_FINAL:
				state = STATE_INITIAL;
				ready = 0 == available;
				break;
			case STATE_INVALID:
			default:
				state = STATE_INVALID;
				throw IOException(WITHDETAILS(L"Invalid state."));			
				break;
			}
		}
	}
	catch (const IOException&)
	{
		state = STATE_INVALID;
		ChunkBufferPool::getInstance()->makeIdle(source);
		throw;
	}
	catch (const Exception& e)
	{
		state = STATE_INVALID;
		ChunkBufferPool::getInstance()->makeIdle(source);
		throw IOException(e->toString());
	}
	catch (...)
	{
		state = STATE_INVALID;
		ChunkBufferPool::getInstance()->makeIdle(source);
		throw IOException(WITHDETAILS(L"Unknown exception."));
	}

	ChunkBufferPool::getInstance()->makeIdle(source);

#undef resetincomplete
}

//********************************************************************

const int Multiplexer::IDENTIFIERLENGTH = 32;
const int Multiplexer::ENDOFFILE = -1;
const int Multiplexer::ENDOFBUFFER = -2;

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
