// Runnable.h

//********************************************************************

#ifndef jlj_lang_Runnable_h
#define jlj_lang_Runnable_h

//********************************************************************

#include <jlj/lang/AbstractRunnable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang RunnableI 
	: public AbstractRunnableI
{
public:
	~RunnableI() = 0;
	RunnableI();

public:
	virtual void run() = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Runnable, ExportedByJLJlang);

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
