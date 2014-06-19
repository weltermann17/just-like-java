// synchronized_.i.h

//********************************************************************

#ifndef synchronized_i_h
#define synchronized_i_h

//********************************************************************

#include <jlj/util/concurrent/locks/synchronized.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(locks)

//********************************************************************
//********************************************************************

synchronized_::~synchronized_() 
{
	lock->unlock();
}

//********************************************************************

synchronized_::synchronized_(LockI* lock) throw (InterruptedException)
: lock(lock)
, locked(true) 
{
	lock->lock();
}

//********************************************************************

void synchronized_::unlock()
{
	locked = false;
}

//********************************************************************

synchronized_::operator bool() const
{
	return locked;
}

//********************************************************************

synchronized_& synchronized_::operator=(const synchronized_&) 
{
	return *this;
}

//********************************************************************

NAMESPACE_END(locks)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
