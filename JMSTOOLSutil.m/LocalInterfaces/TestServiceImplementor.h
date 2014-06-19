// TestServiceImplementor.h

//********************************************************************

#ifndef jmstools_TestServiceImplementor_h
#define jmstools_TestServiceImplementor_h

//********************************************************************

#include <JMSTOOLSutil.h>

#include <jmstools/util/services/AbstractServiceImplementor.h>
using namespace jmstools::util::services;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSTOOLSutil TestServiceImplementorI
	: public AbstractServiceImplementorI
{
protected:
	~TestServiceImplementorI();
	friend class TypedReference<TestServiceImplementorI>;

public:
	TestServiceImplementorI();
	String doService(const String& in) throw (Exception);
	
};

//********************************************************************

StrongReferenceCommonDeclaration(TestServiceImplementor, ExportedByJMSTOOLSutil);
ClassLoaderInformationDeclaration(jmstools::TestServiceImplementor, 179a2e8699bb1be11da5109b6cebab6e, ExportedByJMSTOOLSutil); 

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof




