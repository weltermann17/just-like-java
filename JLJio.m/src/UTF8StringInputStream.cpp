// UTF8StringInputStream.cpp

//********************************************************************

#ifndef UTF8StringInputStream_cpp
#define UTF8StringInputStream_cpp

//********************************************************************

#include <jlj/io/UTF8StringInputStream.h>
#include <jlj/lang/StringBuffer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

UTF8StringInputStreamI::~UTF8StringInputStreamI() throw (IOException)
{}

//********************************************************************

UTF8StringInputStreamI::UTF8StringInputStreamI(UTF8String& utf8string) 
: buffer(utf8string)	
, position(0)
, bookmark(0)
{
	buffersize = buffer->length(); // must be done here because of swap (utf8string is empty now)
	bufferp = constcharptr(buffer);
}

//********************************************************************

UTF8StringInputStreamI::UTF8StringInputStreamI(const UTF8String& utf8string) 
: buffer(utf8string)
, buffersize(utf8string->length())
, position(0)
, bookmark(0)
{
	bufferp = constcharptr(buffer);
}

//********************************************************************

int UTF8StringInputStreamI::available() const throw (IOException)
{
	return buffersize - position;
}

//********************************************************************

void UTF8StringInputStreamI::mark(int) throw (IOException)
{
	bookmark = position;
}

//********************************************************************

bool UTF8StringInputStreamI::markSupported() const
{
	return true;
}

//********************************************************************

void UTF8StringInputStreamI::reset() throw (IOException)
{
	position = bookmark;
}

//********************************************************************

int UTF8StringInputStreamI::skip(int n) throw (IOException)
{
	int last = position;
	position += n;
	if (position < 0 || position >= buffersize) throw IOException(L"UTF8StringInputStreamI::skip() failed"); 
	return position - last;
}

//********************************************************************

int UTF8StringInputStreamI::read(char* result, int offset, int length) throw (IOException)
{
	if (-1 < position && position < buffersize)
	{
		int len = length < buffersize - position ? length : buffersize - position; 
		memcpy(result + offset, bufferp + position, len);
		position += len;
		return len;
	}
	else
	{
		return -1;
	}
}

//********************************************************************

int UTF8StringInputStreamI::tell() const throw (IOException)
{
	return position;
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
