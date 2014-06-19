// TestServiceClient.h

//********************************************************************

#ifndef jmstools_TestServiceClient_h
#define jmstools_TestServiceClient_h

//********************************************************************

#include <JMSTOOLSTESTservicesprovider.h>
#include <jmstools/util/services/ServiceImplementor.h>
using namespace jmstools::util::services;

//********************************************************************

//#include <jlj/lang/String.h>
#include <jlj/jms/QueueSession.h>
#include <jlj/jms/QueueRequestor.h>
#include <jlj/jms/TextMessage.h>
using namespace jlj::jms;
//using namespace jlj::lang;
//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSTOOLSTESTservicesprovider TestServiceClientI 
	: public ObjectI
{
protected:
	~TestServiceClientI();
	friend class TypedReference<TestServiceClientI>;

public:
	TestServiceClientI(QueueSession, const String& queuename, const String& message) throw (Exception);

private:
};

//********************************************************************

StrongReferenceCommonDeclaration(TestServiceClient, ExportedByJMSTOOLSTESTservicesprovider);

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof




