// ServicesProviderInitialContext.h

//********************************************************************

#ifndef jmstools_util_services_ServicesProviderInitialContext_h
#define jmstools_util_services_ServicesProviderInitialContext_h

//********************************************************************

#include <jmstools/util/services/ServicesProvider.h>

//********************************************************************

#include <jlj/lang/ClassLoader.h>
#include <jlj/naming/AbstractContext.h>
using namespace jlj::naming;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

class ExportedByJMSTOOLSutil ServicesProviderInitialContextI
	: public AbstractContextI
{
protected:
	~ServicesProviderInitialContextI();
	friend class TypedReference<ServicesProviderInitialContextI>;

public:
	ServicesProviderInitialContextI();	
	Object lookupClass(const String& name, const String& clazzname) const throw (NamingException);

};

//********************************************************************

StrongReferenceCommonDeclaration(ServicesProviderInitialContext, ExportedByJMSTOOLSutil);
ClassLoaderInformationDeclaration(jmstools::naming::ServicesProviderInitialContext, a95e096b58370f5a0b9ebeb210bdee5a, ExportedByJMSTOOLSutil); 

//********************************************************************

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
