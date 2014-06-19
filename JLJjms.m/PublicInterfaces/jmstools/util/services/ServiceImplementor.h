// ServiceImplementor.h

//********************************************************************

#ifndef jmstools_util_services_ServiceImplementor_h
#define jmstools_util_services_ServiceImplementor_h

//********************************************************************

#include <JMSTOOLSutil.h>

//********************************************************************

#include <jlj/util/Anything.h>
using namespace jlj::lang;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

class ExportedByJMSTOOLSutil ServiceImplementorI
	: public virtual ObjectI
{
public:
	~ServiceImplementorI() = 0;
	ServiceImplementorI();

public:
	virtual String doGet(const String& in) throw (Exception) = 0;
	virtual String doHead(const String& in) throw (Exception) = 0;
	virtual String doOptions(const String& in) throw (Exception) = 0;
	virtual String doPost(const String& in) throw (Exception) = 0;
	virtual String doPut(const String& in) throw (Exception) = 0;
	virtual String doDelete(const String& in) throw (Exception) = 0;

	virtual String doService(const String& in) throw (Exception) = 0;

	virtual void setInParameters(const Anything&) = 0;
	virtual Anything getOutParameters() const = 0;

	virtual void setName(const String& name) = 0;
	virtual String getName() const = 0;

	virtual void setConfiguration(const String& configuration) throw (Exception) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(ServiceImplementor, ExportedByJMSTOOLSutil);

//********************************************************************

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof