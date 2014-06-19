// IntAnything.cpp

//********************************************************************

#ifndef IntAnything_cpp
#define IntAnything_cpp

//********************************************************************

#include <jlj/util/IntAnything.h>
#include <jlj/core/platform/bigendian.h>
#include <jlj/core/platform/LocaleGuard.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

IntAnythingI::~IntAnythingI() 
{}

//********************************************************************

IntAnythingI::IntAnythingI() 
: value(0)
{}

//********************************************************************

IntAnythingI::IntAnythingI(int value_)
: value(value_) 
{}

//********************************************************************

IntAnythingI::IntAnythingI(const String& value_)
: value(0) 
{
	LocaleGuard localeguard;
	value = (int) atof(constcharptr(value_->toUTF8String()));
}

//********************************************************************

IntAnythingI::IntAnythingI(InputStream& in, 
						   Map<int, void*>&) throw (IOException)
{
	in->readComplete((char*)&value, 0, sizeof(int));
	value = bigendian(value);
}

//********************************************************************

int IntAnythingI::type() const 
{
	return Anything::ANY_INT;
}

//********************************************************************

IntAnythingI::operator int() const 
{
	return value;
}

//********************************************************************

IntAnythingI::operator UTF8String() const 
{
	return UTF8String(value);
}

//********************************************************************

int IntAnythingI::size() const
{
	return operator int();
}

//********************************************************************

void IntAnythingI::serialize__(OutputStream& os, 
							   Map<void*, int>& references,
							   Anything) const throw (IOException) 
{
	int type = bigendian(Anything::ANY_INT);
	int i = bigendian(value);
	os->write((char*)&type, sizeof(int));
	os->write((char*)&i, sizeof(int));
}

//********************************************************************

void IntAnythingI::write__(Writer& writer, 
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

void IntAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	buffer->append(UTF8String(value));
}

//********************************************************************

Object IntAnythingI::clone() const 
{
	return new IntAnythingI(value);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
