// AbstractLifetimeImplementation.h

//********************************************************************

#ifndef jlj_lang_lifetimemanagement_AbstractLifetimeImplementation_h
#define jlj_lang_lifetimemanagement_AbstractLifetimeImplementation_h

//********************************************************************

#include <jlj/lang/lifetimemanagement/LifetimeImplementation.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)
NAMESPACE_BEGIN(lifetimemanagement)

//********************************************************************

struct ExportedByJLJlang LifetimeManager;

//********************************************************************

class ExportedByJLJlang AbstractLifetimeImplementationI
	: public LifetimeImplementationI
{
public:
	~AbstractLifetimeImplementationI() throw (Exception) = 0; 
	AbstractLifetimeImplementationI(const String& name) throw (Exception); 

	String getName() const;

protected:
	LifetimeManager getLifetimeManager() throw (Exception);

private:
	String name;

};

//********************************************************************

StrongReferenceCommonDeclaration(AbstractLifetimeImplementation, ExportedByJLJlang);

//********************************************************************

NAMESPACE_END(lifetimemanagement)
NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
