// ReentrantReadWriteLock.cpp

//********************************************************************

#ifndef ReentrantReadWriteLock_cpp
#define ReentrantReadWriteLock_cpp

//********************************************************************

#include <jlj/util/concurrent/locks/ReentrantReadWriteLock.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(locks)

//********************************************************************

ReentrantReadWriteLockI::~ReentrantReadWriteLockI() 
{}

//********************************************************************

ReentrantReadWriteLockI::ReentrantReadWriteLockI() throw (UnsupportedOperationException) 
: readlock(new ReadLock(rwlockrw))
, writelock(new WriteLock(rwlockrw))
{
	throw UnsupportedOperationException(L"ReentrantReadWriteLockI not yet implemented correctly.");
}

//********************************************************************

Lock ReentrantReadWriteLockI::readLock() 
{
	return readlock;
}

//********************************************************************

Lock ReentrantReadWriteLockI::writeLock()
{
	return writelock;
}

//********************************************************************
//********************************************************************

ReentrantReadWriteLockI::ReadLock::ReadLock(pt::rwlock& rwlockrw)
: rwlockr(rwlockrw)
{}

//********************************************************************

void ReentrantReadWriteLockI::ReadLock::lock()
{
	rwlockr.rdlock();
}

//********************************************************************

void ReentrantReadWriteLockI::ReadLock::lockInterruptibly() throw (UnsupportedOperationException)
{
	throw UnsupportedOperationException(L"ReentrantReadWriteLockI::WriteLock::lockInterruptedly()");
}

//********************************************************************

bool ReentrantReadWriteLockI::ReadLock::tryLock()
{
	return tryLock(0);
}

//********************************************************************

bool ReentrantReadWriteLockI::ReadLock::tryLock(int milliseconds) throw (UnsupportedOperationException)
{
	throw UnsupportedOperationException(L"ReentrantReadWriteLockI::ReadLock::tryLock(int milliseconds)");
}

//********************************************************************

void ReentrantReadWriteLockI::ReadLock::unlock() throw (IllegalMonitorStateException)
{
	rwlockr.unlock();
}

//********************************************************************

ReentrantReadWriteLockI::WriteLock::WriteLock(pt::rwlock& rwlockrw)
: rwlockw(rwlockrw)
{}

//********************************************************************

void ReentrantReadWriteLockI::WriteLock::lock()
{
	rwlockw.wrlock();
}

//********************************************************************

void ReentrantReadWriteLockI::WriteLock::lockInterruptibly() throw (UnsupportedOperationException)
{
	throw UnsupportedOperationException(L"ReentrantReadWriteLockI::WriteLock::lockInterruptedly()");
}

//********************************************************************

bool ReentrantReadWriteLockI::WriteLock::tryLock() 
{
	return tryLock(0);
}

//********************************************************************

bool ReentrantReadWriteLockI::WriteLock::tryLock(int milliseconds) throw (UnsupportedOperationException)
{
	throw UnsupportedOperationException(L"ReentrantReadWriteLockI::WriteLock::tryLock(int milliseconds)");
}

//********************************************************************

void ReentrantReadWriteLockI::WriteLock::unlock() throw (IllegalMonitorStateException)
{
	rwlockw.unlock();
}

//********************************************************************

NAMESPACE_END(locks)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
