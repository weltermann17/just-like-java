// TestServiceClientI.cpp

//********************************************************************

#ifndef jmstools_TestServiceClient_cpp
#define jmstools_TestServiceClient_cpp

//********************************************************************

#include <TestServiceClient.h>

//********************************************************************

#include <jlj/lang/StringBuffer.h>
#include <jlj/util/MapAnything.h>
#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

TestServiceClientI::~TestServiceClientI()
{}

//********************************************************************

TestServiceClientI::TestServiceClientI(QueueSession queuesession, 
									   const String& queuename, 
									   const String& message) throw (Exception)
{
	QueueRequestor queuerequestor = queuesession->createQueueRequestor(queuesession->createQueue(queuename));
	TextMessage request = queuesession->createTextMessage(message);
	request->setStringProperty(L"a", L"123");
	request->setStringProperty(L"b", L"456");
	TextMessage reply;
	queuerequestor->request(request, 2000)->downcast(reply);
	StringBuffer buf(reply->getText());
	buf->reverse();
	// cout << buf << endl;
	Anything outparameters = new MapAnythingI();
	Iterator<String> i = reply->getPropertyNames()->iterator();
	while (i->hasNext()) { String name = i->next(); outparameters->put(name, reply->getStringProperty(name)); }
	// cout << outparameters << endl;
}

//********************************************************************

NAMESPACE_END(jmstools)

#endif

// eof