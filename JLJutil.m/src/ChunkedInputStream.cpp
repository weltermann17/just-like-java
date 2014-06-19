// ChunkedInputStreamI.cpp

//********************************************************************

#ifndef ChunkedInputStream_cpp
#define ChunkedInputStream_cpp

//********************************************************************

#include <jlj/util/ChunkedInputStream.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

ChunkedInputStreamI::~ChunkedInputStreamI() throw (IOException)
{
	close();
}

//********************************************************************

ChunkedInputStreamI::ChunkedInputStreamI(InputStream inputstream) throw (IOException)
: inputstream(inputstream)
, chunk(0)
, total(0)
, eof(false)
{}

//********************************************************************

void ChunkedInputStreamI::close() throw (IOException)
{
	delete chunk;
	chunk = 0;
	eof = true;
}

//********************************************************************

int ChunkedInputStreamI::available() const throw (IOException)
{
	return eof ? -1 : 0 != chunk ? chunk->limit : 0;
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
	while (pos < length && !eof)
	{
		if (0 == chunk) 
		{
			chunk = readBuffer();
		}
		else if (0 == chunk->limit)
		{
			delete chunk;
			chunk = readBuffer();
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

	total += pos;

	return pos;
}

//********************************************************************

int ChunkedInputStreamI::tell() const throw (IOException)
{
	return total;
}

//********************************************************************

ChunkBuffer ChunkedInputStreamI::readBuffer() throw (IOException)
{
	ChunkBuffer source = new ChunkBufferI();

	int length;
	inputstream->readComplete((char*) &length, 0, sizeof(int));
	length = bigendian(length);

	if (0 < length && length <= ChunkBufferI::DEFAULTCAPACITY)
	{
		inputstream->readComplete(source->buffer, source->position, length);
		source->position += length;
		source->limit -= length;
	}
	else
	{
		throw IOException(WITHDETAILS(L"Invalid buffer length."));			
	}

	int command;
	inputstream->readComplete((char*) &command, 0, sizeof(int));
	command = bigendian(command);

	if (ChunkBufferI::ENDOFBUFFER == command)
	{
		// ignore
	}
	else if (ChunkBufferI::ENDOFFILE == command)
	{
		eof = true;

		// read ENDOFBUFFER and ignore it
		inputstream->readComplete((char*) &command, 0, sizeof(int));
		command = bigendian(command);
	}
	else
	{
		throw IOException(WITHDETAILS(L"Invalid command."));			
	}

	source->limit = source->position;
	source->position = 0;
	return source;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
