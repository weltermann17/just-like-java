// DoubleAnything.cpp

//********************************************************************

#ifndef DoubleAnything_cpp
#define DoubleAnything_cpp

//********************************************************************

#include <jlj/util/DoubleAnything.h>
#include <jlj/core/platform/bigendian.h>
#include <jlj/core/platform/LocaleGuard.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

DoubleAnythingI::~DoubleAnythingI() 
{}

//********************************************************************

DoubleAnythingI::DoubleAnythingI() 
{}

//********************************************************************

DoubleAnythingI::DoubleAnythingI(double value_)
: value(value_) 
{}

//********************************************************************

DoubleAnythingI::DoubleAnythingI(const String& value_)
: value(0.) 
{
	LocaleGuard localeguard;
	value = atof(constcharptr(value_->toUTF8String()));
}

//********************************************************************

DoubleAnythingI::DoubleAnythingI(InputStream& in, 
								 Map<int, void*>& references) throw (IOException)
{	
	int putback = sizeof(int);
	int tellg = in->tell() - putback;
	in->readComplete((char*)&value, 0, sizeof(double));
	value = bigendian(value);
	references->put(tellg, (void*) this);
}

//********************************************************************

int DoubleAnythingI::type() const 
{
	return Anything::ANY_DOUBLE;
}

//********************************************************************

DoubleAnythingI::operator bool() const 
{
	return 0. != value;
}

//********************************************************************

DoubleAnythingI::operator int() const 
{
	return (int) value;
}

//********************************************************************

DoubleAnythingI::operator double() const 
{
	return value;
}

//********************************************************************

DoubleAnythingI::operator UTF8String() const 
{
	return UTF8String(value);
}

//********************************************************************

int DoubleAnythingI::size() const
{
	return operator int();
}

//********************************************************************

void DoubleAnythingI::serialize__(OutputStream& os,
								  Map<void*, int>& references,
								  Anything) const throw (IOException)
{
	if (serializeReference(os, references, (void*) this))
	{
		int type = bigendian(Anything::ANY_DOUBLE);
		double d = bigendian(value);
		os->write((char*)&type, sizeof(int));
		os->write((char*)&d, sizeof(double));
	}
}

//********************************************************************

void DoubleAnythingI::write__(Writer& writer, 
							  int level, 
							  Map<void*, int>& references) const throw (IOException)
{
	if (this->writeReference(writer, level, references, (void*) this))
	{
		writer->write(String(value));
		writer->write(L"</anything>\n");
	}
}

//********************************************************************

void DoubleAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	buffer->append(UTF8String(value));
}

//********************************************************************

Object DoubleAnythingI::clone() const 
{
	return new DoubleAnythingI(value);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
