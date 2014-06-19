// SocketInputstream->cpp

//********************************************************************

#ifndef jlj_nio_reactor_ChunkedInputStreamI_cpp
#define jlj_nio_reactor_ChunkedInputStreamI_cpp

//********************************************************************

#include <jlj/nio/reactor/ChunkedInputStream.h>
#include <jlj/nio/reactor/Demultiplexer.h>
#include <jlj/nio/reactor/ConnectionLifetimeManager.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
using namespace jlj::util::concurrent;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

ChunkedInputStreamI::~ChunkedInputStreamI() throw (IOException)
{
	close();
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::FINEST, L"ChunkedInputStreamI::~ChunkedInputStreamI()");
}

//********************************************************************

ChunkedInputStreamI::ChunkedInputStreamI(const String& id) throw (IOException)
: readbuffers(new LinkedBlockingQueueI<ChunkBuffer>())
, chunk(0)
, total(0)
, eof(false)
, timeout(ConnectionLifetimeManager::getInstance(id)->getReactor()->getTimeout())
, id(id)
, closed(false)
{
	const int DEFAULTTIMEOUT = 10*1000;
	if (DEFAULTTIMEOUT > timeout) timeout = DEFAULTTIMEOUT;
}

//********************************************************************

void ChunkedInputStreamI::demultiplex()
{
	try
	{
		Demultiplexer demultiplexer = new DemultiplexerI(id, this);
		ConnectionLifetimeManager::getInstance(id)->getExecutorService()->execute(demultiplexer);
	}
	catch (const Exception& e)
	{
		Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::SEVERE, L"ChunkedInputStreamI::demultiplex() : " + e->toString());
		close();
	}
}

//********************************************************************

void ChunkedInputStreamI::close() throw (IOException)
{
	ChunkBufferPool::getInstance()->makeIdle(chunk);
	chunk = 0;
	eof = true;
	closed = true;
}

//********************************************************************

int ChunkedInputStreamI::available() const throw (IOException)
{
	return eof ? -1 : 0 != chunk ? chunk->limit : 0;
}

//********************************************************************

bool ChunkedInputStreamI::isClosed()  
{
	return closed;
}

//********************************************************************

int ChunkedInputStreamI::skip(int n) throw (IOException)
{
	if (0 > n) throw IOException(WITHDETAILS(L"skip() with negative value not supported."));
	for (int i = 0; i < n; ++i)
	{
		char dummy = AbstractInputStreamI::read();
	}
	return n;
}

//********************************************************************

int ChunkedInputStreamI::read(char* result, int offset, int length) throw (IOException)
{
	if (eof) 
	{
		return -1;
	}
	
	int pos = 0;
	while (pos < length)
	{
		if (0 == chunk) 
		{
			chunk = readbuffers->poll(timeout);
		}
		else if (0 == chunk->limit)
		{
			ChunkBufferPool::getInstance()->makeIdle(chunk);
			chunk = readbuffers->poll(timeout);
		}

		if (0 == chunk) 
		{
			eof = true;
			break; 
		}

		int l = chunk->limit < length - pos ? chunk->limit : length - pos;

		memcpy(result + offset + pos, chunk->buffer + chunk->position, l);
		chunk->position += l;
		chunk->limit -= l;

		pos += l;
	}

	total->getAndAdd(pos);

	if (0 != chunk && 0 == chunk->limit)
	{
		ChunkBufferPool::getInstance()->makeIdle(chunk);
		chunk = readbuffers->poll(timeout);

		if (0 == chunk) 
		{
			eof = true; // but return proper length because this read was still ok
		}
	}

	return pos;
}

//********************************************************************

int ChunkedInputStreamI::tell() const throw (IOException)
{
	return total;
}

//********************************************************************

void ChunkedInputStreamI::pipe(ChunkBuffer chunk) 
{
	if (0 != chunk)
	{
		// have a look at java.nio.Buffer.flip()

		if (0 < chunk->position)
		{
			chunk->limit = chunk->position;
			chunk->position = 0;
			readbuffers->put(chunk);
		}
		else
		{
			ChunkBufferPool::getInstance()->makeIdle(chunk);
		}
	}
	else
	{
		readbuffers->put(chunk);
	}
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
