// AtomicBoolean.cpp

//********************************************************************

#ifndef AtomicBoolean_cpp
#define AtomicBoolean_cpp

//********************************************************************

#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
#include <jlj/core/ptypes/pasync.h>
#include <jlj/lang/String.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(atomic)

//********************************************************************

AtomicBooleanI::~AtomicBooleanI() 
{}

//********************************************************************

AtomicBooleanI::AtomicBooleanI(bool newvalue)
{
	pt::pexchange(&value, newvalue ? 1 : 0);
}

//********************************************************************

bool AtomicBooleanI::compareAndSet(bool expect, bool update)
{
	int v = 0;
	int e = expect ? 1 : 0;
	int u = update ? 1 : 0;
	pt::pexchange(&v, value);
	bool b = 1 == v;
	if (b == expect)
	{
		return e == pt::pexchange(&value, u);
	}
	else
	{
		return false;
	}
}

//********************************************************************

bool AtomicBooleanI::get() const
{
	int v = 0;
	pt::pexchange(&v, value);
	return 1 == v;
}

//********************************************************************

bool AtomicBooleanI::getAndSet(bool newvalue)
{
	return 1 == pt::pexchange(&value, newvalue ? 1 : 0);
}

//********************************************************************

void AtomicBooleanI::lazySet(bool newvalue)
{
	pt::pexchange(&value, newvalue ? 1 : 0);
}

//********************************************************************

void AtomicBooleanI::set(bool newvalue)
{
	pt::pexchange(&value, newvalue ? 1 : 0);
}

//********************************************************************

String AtomicBooleanI::toString() const
{
	int v = 0;
	pt::pexchange(&v, value);
	return String(1 == v ? L"true" : L"false");
}

//********************************************************************
//********************************************************************

AtomicBoolean::AtomicBoolean(bool value) 
: StrongReference<AtomicBooleanI>(new AtomicBooleanI(value))
{}

//********************************************************************

AtomicBoolean& AtomicBoolean::operator=(bool value)
{
	get()->set(value);
	return *this;
}

//********************************************************************

AtomicBoolean::operator bool() const
{
	return get()->get();
}

//********************************************************************

bool operator!(const AtomicBoolean& b)
{
	return !b->get();
}

//********************************************************************

NAMESPACE_END(atomic)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
