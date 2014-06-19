// StringReader.cpp

//********************************************************************

#ifndef StringReader_cpp
#define StringReader_cpp

//********************************************************************

#include <jlj/io/StringReader.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

StringReaderI::~StringReaderI() throw (IOException) 
{}

//********************************************************************

StringReaderI::StringReaderI(const String& buffer) 
: buffer(buffer)
, size(buffer->length())
, bookmark(0)
, cursor(0)
{}

//********************************************************************

void StringReaderI::close() throw (IOException)
{
	cursor = -1;
}

//********************************************************************

void StringReaderI::mark(int) throw (IOException) 
{
	bookmark = cursor;
}

//********************************************************************

bool StringReaderI::markSupported() const
{
	return true;
}

//********************************************************************

void StringReaderI::reset() throw (IOException)
{
	cursor = bookmark;
}

//********************************************************************

bool StringReaderI::ready() const throw (IOException)
{
	return -1 != cursor;
}

//********************************************************************

int StringReaderI::skip(int n) throw (IOException)
{
	int last = cursor;

	if (-1 != cursor)
	{
		int newpos = cursor + n;

		if (newpos >= size)
		{
			cursor = -1;
		}
		else
		{
			if (newpos < 0)
			{
				cursor = 0;
			}
			else
			{
				cursor = newpos;
			}
		}
	}
	return last;
}

//********************************************************************

int StringReaderI::read(String& result, int) throw (IOException) 
{
	StringBuffer buf(buffer);
	if (-1 != cursor && cursor < size)
	{
		result = buf->substring(cursor);
	}
	else
	{
		result = String();
	}
	return result->length();
}

//********************************************************************

int StringReaderI::read(wchar_t* result, int offset, int length) throw (IOException)
{
	if (-1 != cursor && cursor < size)
	{
		int l = length - cursor < size ? length - cursor : size; 
		memcpy((char*) (result + offset), (char*) (constwchar_tptr(buffer) + cursor), sizeof(wchar_t) * l);
		cursor += l;
		return l;
	}
	else
	{
		return -1;
	}
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
