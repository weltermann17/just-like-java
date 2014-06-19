// ChunkedOutputStream.cpp

//********************************************************************

#ifndef ChunkedOutputStream_cpp
#define ChunkedOutputStream_cpp

//********************************************************************

#include <jlj/util/ChunkedOutputStream.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

const int ChunkBufferI::DEFAULTCAPACITY = 128*1024 + 2 * sizeof(int);
const int ChunkBufferI::ENDOFFILE = -1;
const int ChunkBufferI::ENDOFBUFFER = -2;

//********************************************************************

ChunkBufferI::~ChunkBufferI()
{
	delete [] buffer;
}

//********************************************************************

ChunkBufferI::ChunkBufferI()
: capacity(DEFAULTCAPACITY)
, limit(DEFAULTCAPACITY)
, position(0)
, buffer(new char[DEFAULTCAPACITY])
{}

//********************************************************************

void ChunkBufferI::clear()
{
	position = 0;
	limit = capacity;
}

//********************************************************************
//********************************************************************

ChunkedOutputStreamI::~ChunkedOutputStreamI() throw (IOException) 
{
	close(); 
}

//********************************************************************

ChunkedOutputStreamI::ChunkedOutputStreamI(OutputStream outputstream) throw (IOException)
: outputstream(outputstream)
, chunk(0)
, total(0)
, closed(false)
{}

//********************************************************************

void ChunkedOutputStreamI::close() throw (IOException) 
{
	if (!closed)
	{
		if (0 != chunk)
		{
			const int requiredsize = 1 * sizeof(int);

			if (requiredsize > chunk->limit) 
			{
				flush();
			}

			int eof = bigendian(ChunkBufferI::ENDOFFILE);
			memcpy(chunk->buffer + chunk->position, (char*) &eof, sizeof(int));
			chunk->position += sizeof(int);
			chunk->limit -= sizeof(int);

			flush();
			delete chunk;	
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
			int endofbuffer = bigendian(ChunkBufferI::ENDOFBUFFER);
			memcpy(chunk->buffer + chunk->position, (char*) &endofbuffer, sizeof(int));
			chunk->position += sizeof(int);
			chunk->limit -= sizeof(int);

			outputstream->write(chunk->buffer, 0, chunk->position);

			delete chunk;
		}

		chunk = new ChunkBufferI();
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

		total += pos;

	}
}

//********************************************************************

int ChunkedOutputStreamI::tell() const throw (IOException)
{
	return total;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
