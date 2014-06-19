// Thread.h

//********************************************************************

#ifndef jlj_lang_Thread_h
#define jlj_lang_Thread_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
#include <jlj/lang/Throwable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang ThreadImpl;
struct ExportedByJLJlang Runnable;

//********************************************************************

/**
*
* this class is "final", you cannot inherit from it, 
* to ensure this the macro "FINAL(classname)" is added to the inheritance list
*
*
* because of the "pool" lifecycle of threads 
* ThreadI must be a delegator, ThreadImpl is the delegate
*/

//********************************************************************

FINALC(ThreadI);

//********************************************************************

class ExportedByJLJlang ThreadI
	: public ObjectI
	, FINAL(ThreadI)
{
protected:
	~ThreadI();
	friend class TypedReference<ThreadI>;

public:

	ThreadI(Runnable) throw (NullPointerException);
	ThreadI(Runnable, const String& name) throw (NullPointerException);

	bool isAlive() const;
	bool isInterrupted() const;
	void interrupt();
	void join(int milliseconds) throw (InterruptedException);
	void join() throw (InterruptedException);
	void start();

	String getName() const;

	void run();

private:
	ThreadImpl* delegate;

}; 

//********************************************************************

struct ExportedByJLJlang Thread : StrongReference<ThreadI> 
{    
	StrongReferenceCommonDerivateImplementation(Thread);

	static Thread currentThread() throw (UnsupportedOperationException);
	static void sleep(int millis) throw (InterruptedException);
	static void yield();

	static void createPool(
		int minsize = 0,
		int maxsize = 0,
		int batchsize = 0,
		int addtimeout = 0,
		int addtimeoutatmaxsize = 0,
		int removetimeout = 0,
		int destroytimeout = 0)	throw (NullPointerException) ;
										    
	static void destroyPool();

};

//********************************************************************

ExportedByJLJlang void initializeThread();

//********************************************************************

ExportedByJLJlang void initializeThreadPool(); // call after the function above

//********************************************************************

ExportedByJLJlang void finalizeThreadPool(); 

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
