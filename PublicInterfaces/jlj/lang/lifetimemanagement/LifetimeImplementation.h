// LifetimeImplementation.h

//********************************************************************

#ifndef jlj_lang_lifetimemanagement_LifetimeImplementation_h
#define jlj_lang_lifetimemanagement_LifetimeImplementation_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Throwable.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)
NAMESPACE_BEGIN(lifetimemanagement)

//********************************************************************

class ExportedByJLJlang LifetimeImplementationI
	: public virtual ObjectI
{
public:
	virtual ~LifetimeImplementationI() throw (Exception) = 0; 
	LifetimeImplementationI() throw (Exception); 

public:
	virtual void initialize() throw (Exception) = 0;
	virtual void finalize() throw (Exception) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(LifetimeImplementation, ExportedByJLJlang);

//********************************************************************

NAMESPACE_END(lifetimemanagement)
NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
