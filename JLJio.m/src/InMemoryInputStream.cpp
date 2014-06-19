// InMemoryInputStream.cpp

//********************************************************************

#ifndef InMemoryInputStream_cpp
#define InMemoryInputStream_cpp

//********************************************************************

#include <jlj/io/InMemoryInputStream.h>
#include <jlj/lang/StringBuffer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

InMemoryInputStreamI::~InMemoryInputStreamI() throw (IOException)
{}

//********************************************************************

InMemoryInputStreamI::InMemoryInputStreamI(const char* buffer, 
										   int buffersize) throw (IOException) 
										   : buffer(buffer)
										   , buffersize(buffersize)
										   , position(0)
										   , bookmark(0)
{
	if (0 == buffer) throw IOException(L"Invalid buffer");
}

//********************************************************************

int InMemoryInputStreamI::available() const throw (IOException)
{
	return buffersize - position;
}

//********************************************************************

void InMemoryInputStreamI::mark(int) throw (IOException)
{
	bookmark = position;
}

//********************************************************************

bool InMemoryInputStreamI::markSupported() const
{
	return true;
}

//********************************************************************

void InMemoryInputStreamI::reset() throw (IOException)
{
	position = bookmark;
}

//********************************************************************

int InMemoryInputStreamI::skip(int n) throw (IOException)
{
	int last = position;
	position += n;
	if (position < 0) throw IOException(L"InMemoryInputStreamI::skip() failed"); 
	return position - last;
}

//********************************************************************

int InMemoryInputStreamI::read(char* result, int offset, int length) throw (IOException)
{
	if (0 != buffer && 0 != result)
	{
		if (position + length <= buffersize)
		{
			memcpy(result + offset, buffer + position, length);
			position += length;
			return length;
		}
		else
		{
			if (position < buffersize)
			{
				long delta = buffersize - position;
				memcpy(result + offset, buffer + position, delta);
				position += delta;
				return delta;
			}
			else
				return -1;
		}
	}
	else 
	{
		StringBuffer msg;
		msg->append(L"buffer = ")->append((buffer ? L"ok" : L"0"))->append(L", result = ")->append((result ? L"ok" : L"0"));
		throw IOException(msg->toString());
	}
}

//********************************************************************

int InMemoryInputStreamI::tell() const throw (IOException)
{
	return position;
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
