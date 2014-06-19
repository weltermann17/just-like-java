// AtomicInteger.cpp

//********************************************************************

#ifndef AtomicInteger_cpp
#define AtomicInteger_cpp

//********************************************************************

#include <jlj/util/concurrent/atomic/AtomicInteger.h>
#include <jlj/core/ptypes/pasync.h>
#include <jlj/lang/String.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(atomic)

//********************************************************************

AtomicIntegerI::~AtomicIntegerI() 
{}

//********************************************************************

AtomicIntegerI::AtomicIntegerI(int newvalue)
{
	pt::pexchange(&value, newvalue);
}

//********************************************************************

int AtomicIntegerI::addAndGet(int delta)
{
	int v = 0;
	pt::pexchange(&v, value);
	pt::pexchange(&value, v + delta);
	return v + delta;
}

//********************************************************************

bool AtomicIntegerI::compareAndSet(int expect, int update)
{
	int v = 0;
	pt::pexchange(&v, value);
	if (v == expect)
	{
		return expect == pt::pexchange(&value, update);
	}
	else
	{
		return false;
	}
}

//********************************************************************

int AtomicIntegerI::decrementAndGet()
{
	return pt::pdecrement(&value);
}

//********************************************************************

int AtomicIntegerI::get() const
{
	int v = 0;
	pt::pexchange(&v, value);
	return v;
}

//********************************************************************

int AtomicIntegerI::getAndAdd(int delta)
{
	int v = 0;
	pt::pexchange(&v, value);
	pt::pexchange(&value, v + delta);
	return v;
}

//********************************************************************

int AtomicIntegerI::getAndDecrement()
{
	int v = 0;
	pt::pexchange(&v, value);
	pt::pdecrement(&value);
	return v;
}

//********************************************************************

int AtomicIntegerI::getAndIncrement()
{
	int v = 0;
	pt::pexchange(&v, value);
	pt::pincrement(&value);
	return v;
}

//********************************************************************

int AtomicIntegerI::getAndSet(int newvalue)
{
	return pt::pexchange(&value, newvalue);
}

//********************************************************************

int AtomicIntegerI::incrementAndGet()
{
	return pt::pincrement(&value);
}

//********************************************************************
//********************************************************************

void AtomicIntegerI::lazySet(int newvalue)
{
	pt::pexchange(&value, newvalue);
}

//********************************************************************

void AtomicIntegerI::set(int newvalue)
{
	pt::pexchange(&value, newvalue);
}

//********************************************************************

String AtomicIntegerI::toString() const
{
	int v = 0;
	pt::pexchange(&v, value);
	return String(v);
}

//********************************************************************
//********************************************************************

AtomicInteger::AtomicInteger(int value) 
: StrongReference<AtomicIntegerI>(new AtomicIntegerI(value))
{}

//********************************************************************

AtomicInteger& AtomicInteger::operator=(int value)
{
	get()->set(value);
	return *this;
}

//********************************************************************

int AtomicInteger::operator++()
{
	return get()->incrementAndGet();
}

//********************************************************************

int AtomicInteger::operator++(int)
{
	return get()->getAndIncrement();
}

//********************************************************************

int AtomicInteger::operator--()
{
	return get()->decrementAndGet();
}

//********************************************************************

int AtomicInteger::operator--(int)
{
	return get()->getAndDecrement();
}

//********************************************************************

AtomicInteger::operator int() const
{
	return get()->get();
}

//********************************************************************

NAMESPACE_END(atomic)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
