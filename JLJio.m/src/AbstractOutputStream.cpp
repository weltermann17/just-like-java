// AbstractOutputStream.cpp

//********************************************************************

#ifndef AbstractOutputStream_cpp
#define AbstractOutputStream_cpp

//********************************************************************

#include <jlj/io/AbstractOutputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

AbstractOutputStreamI::~AbstractOutputStreamI() throw (IOException)
{
	close();
}

//********************************************************************

AbstractOutputStreamI::AbstractOutputStreamI() 
{}

//********************************************************************

void AbstractOutputStreamI::close() throw (IOException)
{}

//********************************************************************

void AbstractOutputStreamI::flush() throw (IOException)
{}

//********************************************************************

void AbstractOutputStreamI::write(const char* value, int length) throw (IOException)
{
	write(value, 0, length);
}

//********************************************************************

void AbstractOutputStreamI::write(char c) throw (IOException)
{
	write(&c, 0, 1);
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
