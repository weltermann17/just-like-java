// AbstractRunnable.h

//********************************************************************

#ifndef jlj_lang_AbstractRunnable_h
#define jlj_lang_AbstractRunnable_h

//********************************************************************

#include <jlj/lang/Thread.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang AbstractRunnableI 
	: public virtual ObjectI
{
public:
	virtual ~AbstractRunnableI() = 0;
	AbstractRunnableI();

public:
	Thread currentThread();

private:
	void setCurrentThread(Thread);
	void resetCurrentThread();
	friend class ExportedByJLJlang ThreadI;
	friend class ExportedByJLJlang ThreadImpl;

	Thread thread;

};

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
