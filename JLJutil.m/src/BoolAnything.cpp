// BoolAnything.cpp

//********************************************************************

#ifndef BoolAnything_cpp
#define BoolAnything_cpp

//********************************************************************

#include <jlj/util/BoolAnything.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

BoolAnythingI::~BoolAnythingI() 
{}

//********************************************************************

BoolAnythingI::BoolAnythingI() 
: value(L'\0')
{}

//********************************************************************

BoolAnythingI::BoolAnythingI(bool value)
: value(value) 
{}

//********************************************************************

BoolAnythingI::BoolAnythingI(InputStream& in, 
							 Map<int, void*>&) throw (IOException)
{
	int i;
	in->readComplete((char*)&i, 0, sizeof(int));
	i = bigendian(i);
	value = 0 == i ? false : true;
}

//********************************************************************

int BoolAnythingI::type() const 
{
	return Anything::ANY_BOOL;
}

//********************************************************************

BoolAnythingI::operator bool() const 
{
	return value;
}

//********************************************************************

BoolAnythingI::operator int() const 
{
	return value;
}

//********************************************************************

int BoolAnythingI::size() const 
{
	return value ? 1 : 0;
}

//********************************************************************

void BoolAnythingI::serialize__(OutputStream& os,
								Map<void*, int>& references,
								Anything) const throw (IOException) 
{
	int type = bigendian(Anything::ANY_BOOL);
	int i = value ? 1 : 0; 
	i = bigendian(i);
	os->write((char*)&type, sizeof(int));
	os->write((char*)&i, sizeof(int));
}

//********************************************************************

void BoolAnythingI::write__(Writer& writer, 
							int level, 
							Map<void*, int>& references) const throw (IOException)
{
	if (this->writeReference(writer, level, references, (void*) this))
	{
		writer->write(value ? L"true" : L"false");
		writer->write(L"</anything>\n");
	}
}

//********************************************************************

void BoolAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	buffer->append(value ? "true" : "false");
}

//********************************************************************

Object BoolAnythingI::clone() const 
{
	return new BoolAnythingI(value);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
