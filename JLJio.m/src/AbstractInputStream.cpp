// AbstractInputStream.cpp

//********************************************************************

#ifndef AbstractInputStream_cpp
#define AbstractInputStream_cpp

//********************************************************************

#include <jlj/io/AbstractInputStream.h>
#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/lang/StringBuffer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

AbstractInputStreamI::~AbstractInputStreamI() throw (IOException)
{
	close();
}

//********************************************************************

AbstractInputStreamI::AbstractInputStreamI() 
{}

//********************************************************************

int AbstractInputStreamI::available() const throw (IOException)
{
	return 0;
}

//********************************************************************

void AbstractInputStreamI::mark(int) throw (IOException)
{}

//********************************************************************

bool AbstractInputStreamI::markSupported() const
{
	return false;
}

//********************************************************************

int AbstractInputStreamI::skip(int) throw (IOException)
{
	throw IOException(WITHDETAILS(L"AbstractInputStreamI::skip() not supported."));
}

//********************************************************************

int AbstractInputStreamI::tell() const throw (IOException)
{
	return 0;
}

//********************************************************************

void AbstractInputStreamI::close() throw (IOException)
{}

//********************************************************************

void AbstractInputStreamI::reset() throw (IOException)
{}

//********************************************************************

char AbstractInputStreamI::read() throw (IOException)
{
	char result = 0;
	read(&result, 0, 1);
	return result;
}

//********************************************************************

int AbstractInputStreamI::readComplete(char* result, int offset, int length) throw (IOException)
{
	if (0 > length) 
		throw IOException(WITHDETAILS(L"Invalid negative length : " + String(length)));

	int n = 0;
	int total = 0;

	while (total < length && 0 < (n = read(result, offset + total, length - total)))
	{
		total += n;
	}

	if (total < length) 
	{
		StringBuffer buf;
		buf->append(L"AbstractInputStreamI::readComplete() failed : ");
		buf->append(total);
		buf->append(L" of ");
		buf->append(length);
		throw IOException(WITHDETAILS(buf->toString()));
	}
	return total;
}

//********************************************************************

int AbstractInputStreamI::read(UTF8String& result, int expectedsize) throw (IOException)
{
	/**
	* this is not very efficient as it reads char by char,
	* but this is necessary for SocketInputStream.
	*
	* It should be overwritten for other InputStreams.
	*/

	char buf[2]; buf[1] = 0;
	int n = 0;
	int total = 0;

	UTF8StringBuffer buffer;
	if (0 < expectedsize) buffer->ensureCapacity(expectedsize);

	while (0 < read(buf, 0, 1))
	{
		if (0 == buf[0]) break; // null terminated UTF8String 
		total += 1;
		buffer->append(buf[0]);
	}
	result = UTF8String(buffer); // consumes buffer	
	return total;
}

//********************************************************************

int AbstractInputStreamI::read(char* result, int length) throw (IOException)
{
	return read(result, 0, length);
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
