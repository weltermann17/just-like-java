// ReentrantLock.h

//********************************************************************

#ifndef jlj_util_ReentrantLock_h
#define jlj_util_ReentrantLock_h

//********************************************************************

#include <jlj/util/concurrent/locks/Lock.h>
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

class ExportedByJLJutil ReentrantLockI 
	: public LockI
{
protected:
	~ReentrantLockI();
	friend class TypedReference<ReentrantLockI>;

public:	
	ReentrantLockI();

public:
	void lock();	
	void lockInterruptibly() throw (InterruptedException, UnsupportedOperationException);	
	bool tryLock();
	bool tryLock(int milliseconds) throw (InterruptedException, UnsupportedOperationException);
	void unlock() throw (IllegalMonitorStateException);

	int getHoldCount() const;
	bool isHeldByCurrentThread() const;
	bool isLocked() const;

	String toString() const;

private:
#if defined (_WINDOWS_SOURCE)
	int owner;
#else
	pthread_t owner;
#endif
	int holds;
	int acquired;
	pt::timedsem timedsemaphore;

};

//********************************************************************

StrongReferenceCommonDeclaration(ReentrantLock, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(locks)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
