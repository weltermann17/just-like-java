// ReentrantReadWriteLock.h

//********************************************************************

#ifndef jlj_util_concurrent_locks_ReentrantReadWriteLock_h
#define jlj_util_concurrent_locks_ReentrantReadWriteLock_h

//********************************************************************

#include <jlj/util/concurrent/locks/ReadWriteLock.h>
#include <jlj/util/concurrent/locks/synchronized.h>
#include <jlj/core/ptypes/pasync.h>
#include <jlj/lang/Throwable.h>
using jlj::lang::IllegalMonitorStateException;
using jlj::lang::UnsupportedOperationException;
#include <jlj/lang/String.h>
using jlj::lang::String;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(locks)

//********************************************************************

/**
*
* not yet implemented correctly
*
*/

class ExportedByJLJutil ReentrantReadWriteLockI 
	: public ReadWriteLockI
{
protected:
	~ReentrantReadWriteLockI();
	friend class TypedReference<ReentrantReadWriteLockI>;

public:	
	ReentrantReadWriteLockI() throw (UnsupportedOperationException);

public:
	Lock readLock();	
	Lock writeLock();	

private:
	pt::rwlock rwlockrw;
	Lock readlock;
	Lock writelock;

private:
	class ExportedByJLJutil ReadLock : public LockI
	{
	public:
		ReadLock(pt::rwlock& rwlockrw);

		void lock();	
		void lockInterruptibly() throw (UnsupportedOperationException);	
		bool tryLock();
		bool tryLock(int milliseconds) throw (UnsupportedOperationException);
		void unlock() throw (IllegalMonitorStateException);

	private:
		pt::rwlock& rwlockr;

	};

	class ExportedByJLJutil WriteLock : public LockI
	{
	public:
		WriteLock(pt::rwlock& rwlockrw);

		void lock();	
		void lockInterruptibly() throw (UnsupportedOperationException);	
		bool tryLock();
		bool tryLock(int milliseconds) throw (UnsupportedOperationException);
		void unlock() throw (IllegalMonitorStateException);

	private:
		pt::rwlock& rwlockw;

	};

};

//********************************************************************

StrongReferenceCommonDeclaration(ReentrantReadWriteLock, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(locks)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
