// ChunkedOutputStream.cpp

//********************************************************************

#ifndef jlj_nio_reactor_ChunkedOutputStream_cpp
#define jlj_nio_reactor_ChunkedOutputStream_cpp

//********************************************************************

#include <jlj/nio/reactor/ChunkedOutputStream.h>
#include <jlj/nio/reactor/ConnectionLifetimeManager.h>
#include <jlj/nio/reactor/Multiplexer.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

ChunkedOutputStreamI::~ChunkedOutputStreamI() throw (IOException) 
{
	close(); 
	delete [] ids;
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::FINEST, L"jlj::nio::reactor::ChunkedOutputStreamI::~ChunkedOutputStreamI()");
}

//********************************************************************

ChunkedOutputStreamI::ChunkedOutputStreamI(const String& id,
										   const String& streamid) throw (IOException)
										   : id(id)
										   , ids(new char[Multiplexer::IDENTIFIERLENGTH * 2])
										   , chunk(0)
										   , total(0)
										   , closed(false)
{
	int l = Multiplexer::IDENTIFIERLENGTH;
	memcpy(ids, constcharptr(id), l);
	memcpy(ids + l, constcharptr(streamid), l);
}

//********************************************************************

void ChunkedOutputStreamI::close() throw (IOException) 
{
	if (!closed)
	{
		if (0 != chunk)
		{
			const int requiredsize = 2 * sizeof(int);

			if (requiredsize > chunk->limit) 
			{
				flush();
			}

			if (0 == chunk->position) 
			{
				int l = Multiplexer::IDENTIFIERLENGTH * 2;

				memcpy(chunk->buffer + chunk->position, ids, l);
				chunk->position += l;
				chunk->limit -= l;
			}

			int eof = bigendian(Multiplexer::ENDOFFILE);
			memcpy(chunk->buffer + chunk->position, (char*) &eof, sizeof(int));
			chunk->position += sizeof(int);
			chunk->limit -= sizeof(int);

			flush();
			ChunkBufferPool::getInstance()->makeIdle(chunk);	
			chunk = 0;

			closed = true;
		}
	}
}

//********************************************************************

void ChunkedOutputStreamI::flush() throw (IOException) 
{
	if (!closed)	
	{
		if (0 != chunk) 
		{
			int endofbuffer = bigendian(Multiplexer::ENDOFBUFFER);
			memcpy(chunk->buffer + chunk->position, (char*) &endofbuffer, sizeof(int));
			chunk->position += sizeof(int);
			chunk->limit -= sizeof(int);

			// flip
			chunk->limit = chunk->position;
			chunk->position = 0;

			// the first Multiplexer::IDENTIFIERLENGTH bytes contain the connectionid 
			// needed by ReactorI::call(), we will not send them to the remote site

			chunk->position += Multiplexer::IDENTIFIERLENGTH;
			chunk->limit -= Multiplexer::IDENTIFIERLENGTH;

			if (testIfStateSet(ICS)) 
			{
				closed = true;
				throw IOException(WITHDETAILS(L"Internal send already closed."));
			}

			ConnectionLifetimeManager::getInstance(id)->getSendBuffers()->put(chunk);
		}

		chunk = ChunkBufferPool::getInstance()->get();
	}
}

//********************************************************************

void ChunkedOutputStreamI::write(const char* value, 
								 int offset, 
								 int length) throw (IOException) 
{
	if (!closed)
	{
		const int requiredsize = 2 * sizeof(int);

		int pos = 0;
		while (pos < length)
		{
			if (0 == chunk || requiredsize >= chunk->limit)
			{
				flush();

				int l = Multiplexer::IDENTIFIERLENGTH * 2;

				memcpy(chunk->buffer + chunk->position, ids, l);
				chunk->position += l;
				chunk->limit -= l;
			}

			int l = (chunk->limit - requiredsize) < (length - pos) 
				? chunk->limit - requiredsize 
				: length - pos;

			int nsl = bigendian(l);
			memcpy(chunk->buffer + chunk->position, (char*) &nsl, sizeof(int));
			chunk->position += sizeof(int);
			chunk->limit -= sizeof(int);

			memcpy(chunk->buffer + chunk->position, value + offset + pos, l);
			chunk->position += l;
			chunk->limit -= l;

			pos += l;
		}

		total += length;
	}
}

//********************************************************************

int ChunkedOutputStreamI::tell() const throw (IOException)
{
	return total;
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
