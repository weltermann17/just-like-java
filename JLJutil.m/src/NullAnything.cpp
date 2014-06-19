// NullAnything.cpp

//********************************************************************

#ifndef NullAnything_cpp
#define NullAnything_cpp

//********************************************************************

#include <jlj/util/NullAnything.h>

//********************************************************************

#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

NullAnythingI::~NullAnythingI() 
{}

//********************************************************************

NullAnythingI::NullAnythingI() 
{}

//********************************************************************

int NullAnythingI::type() const 
{
	return Anything::ANY_NULL;
}

//********************************************************************

void NullAnythingI::serialize__(OutputStream& os,
								Map<void*, int>&,
								Anything) const throw (IOException) 
{
	int type = bigendian(Anything::ANY_NULL);
	os->write((char*)&type, sizeof(int));
}

//********************************************************************

void NullAnythingI::write__(Writer& writer, 
							int level,
							Map<void*, int>&) const throw (IOException)
{
	indent(writer, level);
	writer->write(L"<anything type=\"Null\" />\n");
}

//********************************************************************

void NullAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	buffer->append("null");
}

//********************************************************************

Object NullAnythingI::clone() const 
{
	return new NullAnythingI();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
