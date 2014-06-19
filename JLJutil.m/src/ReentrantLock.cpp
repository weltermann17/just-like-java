// ReentrantLock.cpp

//********************************************************************

#ifndef ReentrantLock_cpp
#define ReentrantLock_cpp

//********************************************************************

#include <jlj/util/concurrent/locks/ReentrantLock.h>

//********************************************************************

#include <jlj/lang/Class.h>
#include <jlj/lang/StringBuffer.h>
using jlj::lang::StringBuffer;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(locks)

//********************************************************************

ReentrantLockI::~ReentrantLockI() 
{}

//********************************************************************

ReentrantLockI::ReentrantLockI() 
: timedsemaphore(1)
, owner(0)
, holds(0)
, acquired(0)
{}

//********************************************************************

void ReentrantLockI::lockInterruptibly() throw (InterruptedException, UnsupportedOperationException)
{
	throw UnsupportedOperationException(L"ReentrantLockI::lockInterruptibly()");
}

//********************************************************************

void ReentrantLockI::unlock() throw (IllegalMonitorStateException)
{
	if (!pt::pthrequal(owner))
	{
		throw IllegalMonitorStateException(L"ReentrantLockI::unlock() : thread is not owner"); 
	}
	else
	{
		if (0 == pt::pdecrement(&holds))
		{
			owner = 0;		
			if (1 == pt::pexchange(&acquired, 0))
			{
				timedsemaphore.signal();
			}
			else
			{
				throw IllegalMonitorStateException(L"ReentrantLockI::unlock() : not locked"); 
			}
		}
	}
}

//********************************************************************

bool ReentrantLockI::tryLock() 
{
	return tryLock(0);
}

//********************************************************************

bool ReentrantLockI::tryLock(int milliseconds) throw (InterruptedException, UnsupportedOperationException)
{
	if (0 > milliseconds) return false;

    if (pt::pthrequal(owner))
	{
		pt::pincrement(&holds);
		return true;
	}
	else 
	{
		bool ok = timedsemaphore.wait(milliseconds);
		if (ok)
		{
			pt::pexchange(&acquired, 1);
			owner = pt::pthrself();
			pt::pexchange(&holds, 1);
		}
		return ok;
	}		
}

//********************************************************************

void ReentrantLockI::lock()
{
    if (pt::pthrequal(owner))
	{
		pt::pincrement(&holds);
	}
	else 
	{
		timedsemaphore.wait();

		pt::pexchange(&acquired, 1);
		owner = pt::pthrself();
		pt::pexchange(&holds, 1);
	}		
}

//********************************************************************

int ReentrantLockI::getHoldCount() const
{
	int localholds = 0;
	pt::pexchange(&localholds, holds);
	return localholds;
}

//********************************************************************

bool ReentrantLockI::isHeldByCurrentThread() const
{
	return pt::pthrequal(owner);
}

//********************************************************************

bool ReentrantLockI::isLocked() const
{
	int localacquired = 0;
	pt::pexchange(&localacquired, acquired);
	return 0 < localacquired;
}

//********************************************************************

String ReentrantLockI::toString() const
{
	StringBuffer result;
	result->append(L"<");
	result->append(getClass()->toString());
	result->append(L" locked=\"");
	result->append(isLocked() ? L"true" : L"false");
	result->append(L"\" holders=\"");
	result->append(getHoldCount());
	result->append(L"\" isheldbycurrentthread=\"");
	result->append(isHeldByCurrentThread() ? L"true" : L"false");
	result->append(L"\" />");
	return String(result);
}

//********************************************************************

NAMESPACE_END(locks)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
