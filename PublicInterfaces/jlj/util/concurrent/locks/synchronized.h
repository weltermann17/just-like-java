// synchronized.h

//********************************************************************

#ifndef synchronized_h
#define synchronized_h

//********************************************************************

#include <jlj/util/concurrent/locks/Lock.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(locks)

//********************************************************************

class ExportedByJLJutil synchronized_
{
public:
	~synchronized_(); // final
	synchronized_(LockI*) throw (InterruptedException);

	void unlock();
	operator bool() const;

private:
	synchronized_& operator=(const synchronized_&);

	LockI* lock;
	bool locked;

};

//********************************************************************

NAMESPACE_END(locks)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

/**
* to allow "java-like" synchronized blocks, eg:
*
*	void f()
*	{
*		synchronized(reentrantlock)
*		{
*			// do something critical here
*		}
*	}
*/

#define synchronized(lock) for (jlj::util::concurrent::locks::synchronized_ __sync__##__LINE__ = lock.get();  __sync__##__LINE__; __sync__##__LINE__.unlock())

//********************************************************************

#endif

// eof
