// Lock.h

//********************************************************************

#ifndef jlj_util_concurrent_locks_Lock_h
#define jlj_util_concurrent_locks_Lock_h

//********************************************************************

#include <jlj/lang/Object.h>
using jlj::lang::ObjectI;
using jlj::lang::Object;
#include <jlj/lang/Throwable.h>
using jlj::lang::InterruptedException;
using jlj::lang::UnsupportedOperationException;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(locks)

//********************************************************************

class ExportedByJLJutil LockI
	: public ObjectI
{
public:
	~LockI() = 0;
	LockI();

public:
	virtual void lock() = 0;	
	virtual void lockInterruptibly() throw (InterruptedException, UnsupportedOperationException) = 0;	
	virtual bool tryLock() = 0;
	virtual bool tryLock(int milliseconds) throw (InterruptedException, UnsupportedOperationException) = 0;
	virtual void unlock() = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Lock, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(locks)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
