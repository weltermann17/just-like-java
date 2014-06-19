// AbstractWriter.cpp

//********************************************************************

#ifndef AbstractWriter_cpp
#define AbstractWriter_cpp

//********************************************************************

#include <jlj/io/AbstractWriter.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

AbstractWriterI::~AbstractWriterI() throw (IOException)
{
	close();
}

//********************************************************************

AbstractWriterI::AbstractWriterI() 
{}

//********************************************************************

void AbstractWriterI::close() throw (IOException)
{}

//********************************************************************

void AbstractWriterI::flush() throw (IOException)
{}

//********************************************************************

void AbstractWriterI::write(const wchar_t* value) throw (IOException)
{
	write(value, 0, wcslen(value));
}

//********************************************************************

void AbstractWriterI::write(wchar_t c) throw (IOException)
{
	wchar_t buf[2];
	buf[0] = c;
	buf[1] = L'\0';
	write(buf);
}

//********************************************************************

void AbstractWriterI::write(const String& value) throw (IOException)
{
	write(value, 0, value->length());
}

//********************************************************************

void AbstractWriterI::write(const String& value, int offset, int length) throw (IOException)
{
	return write(constwchar_tptr(value), offset, length);
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
