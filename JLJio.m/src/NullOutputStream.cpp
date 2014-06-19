// NullOutputStream.cpp

//********************************************************************

#ifndef NullOutputStream_cpp
#define NullOutputStream_cpp

//********************************************************************

#include <jlj/io/NullOutputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

NullOutputStreamI::~NullOutputStreamI() throw (IOException)
{}

//********************************************************************

NullOutputStreamI::NullOutputStreamI() 
: position(0)
{}

//********************************************************************

void NullOutputStreamI::write(const char* value, int offset, int length) throw (IOException)
{
	position += length;
}

//********************************************************************

int NullOutputStreamI::tell() const throw (IOException)
{
	if(0 > position) throw IOException(L"File is too large"); 
	return position;
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
