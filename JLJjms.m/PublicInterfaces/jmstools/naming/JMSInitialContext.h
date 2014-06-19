// JMSInitialContext.h

//********************************************************************

#ifndef jmstools_naming_JMSInitialContext_h
#define jmstools_naming_JMSInitialContext_h

//********************************************************************

#include <JMSTOOLSutil.h>

//********************************************************************

#include <jlj/lang/ClassLoader.h>
#include <jlj/naming/AbstractContext.h>
using namespace jlj::naming;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(naming)

//********************************************************************

class ExportedByJMSTOOLSutil JMSInitialContextI
	: public AbstractContextI
{
protected:
	~JMSInitialContextI();
	friend class TypedReference<JMSInitialContextI>;

public:
	JMSInitialContextI();	
	Object lookupClass(const String& name, const String& clazzname) const throw (NamingException);
	void loadBindings(const String& configuration) throw (Exception);

};

//********************************************************************

StrongReferenceCommonDeclaration(JMSInitialContext, ExportedByJMSTOOLSutil);
ClassLoaderInformationDeclaration(jmstools::naming::JMSInitialContext, d67e0837626fe9efa63ad5cec592cbe4, ExportedByJMSTOOLSutil); 

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
