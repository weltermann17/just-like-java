// AbstractAbstractServiceImplementor.h

//********************************************************************

#ifndef jmstools_util_services_AbstractServiceImplementor_h
#define jmstools_util_services_AbstractServiceImplementor_h

//********************************************************************

#include <jmstools/util/services/ServiceImplementor.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

class ExportedByJMSTOOLSutil AbstractServiceImplementorI
	: public ServiceImplementorI
{
public:
	~AbstractServiceImplementorI() = 0;
	AbstractServiceImplementorI();

public:
	String doGet(const String& in) throw (Exception);
	String doHead(const String& in) throw (Exception);
	String doOptions(const String& in) throw (Exception);
	String doPost(const String& in) throw (Exception);
	String doPut(const String& in) throw (Exception);
	String doDelete(const String& in) throw (Exception);
	String doService(const String& in) throw (Exception);

	void setInParameters(const Anything&);
	Anything getOutParameters() const;

	void setName(const String& name);
	String getName() const;

	void setConfiguration(const String& configuration) throw (Exception);

protected:
	Anything getInParameters() const;
	Logger logger;

private:
	String name;
	Anything inparameters;
	Anything outparameters;

};

//********************************************************************

StrongReferenceCommonDeclaration(AbstractServiceImplementor, ExportedByJMSTOOLSutil);

//********************************************************************

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof