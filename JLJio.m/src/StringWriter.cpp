// StringWriter.cpp

//********************************************************************

#ifndef StringWriter_cpp
#define StringWriter_cpp

//********************************************************************

#include <jlj/io/StringWriter.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

StringWriterI::~StringWriterI() throw (IOException)
{}

//********************************************************************

StringWriterI::StringWriterI() 
{}

//********************************************************************

StringWriterI::StringWriterI(int initialcapacity) 
: buffer(initialcapacity)
{}

//********************************************************************

String StringWriterI::toString() const
{
	return buffer->toString();
}

//********************************************************************

void StringWriterI::write(const wchar_t* value, int offset, int length) throw (IOException)
{
	buffer->append(value, offset, length);
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
