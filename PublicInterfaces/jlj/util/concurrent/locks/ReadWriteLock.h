// ReadWriteLock.h

//********************************************************************

#ifndef jlj_util_concurrent_locks_ReadWriteLock_h
#define jlj_util_concurrent_locks_ReadWriteLock_h

//********************************************************************

#include <jlj/util/concurrent/locks/Lock.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(locks)

//********************************************************************

class ExportedByJLJutil ReadWriteLockI
	: public ObjectI
{
public:
	~ReadWriteLockI() = 0;
	ReadWriteLockI();

public:
	virtual Lock readLock() = 0;	
	virtual Lock writeLock() = 0;	

};

//********************************************************************

StrongReferenceCommonDeclaration(ReadWriteLock, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(locks)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
