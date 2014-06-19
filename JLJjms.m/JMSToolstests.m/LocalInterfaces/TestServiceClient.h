// TestServiceClient.h

//********************************************************************

#ifndef jmstools_TestServiceClient_h
#define jmstools_TestServiceClient_h

//********************************************************************

#include <JMSToolstests.h>

//********************************************************************

#include <jlj/jms/QueueSession.h>
#include <jlj/jms/QueueRequestor.h>
#include <jlj/jms/TextMessage.h>
using namespace jlj::jms;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSToolstests TestServiceClientI 
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

StrongReferenceCommonDeclaration(TestServiceClient, ExportedByJMSToolstests);

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof




