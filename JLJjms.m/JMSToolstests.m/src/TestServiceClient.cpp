// TestServiceClientI.cpp

//********************************************************************

#ifndef jmstools_TestServiceClient_cpp
#define jmstools_TestServiceClient_cpp

//********************************************************************

#include <TestServiceClient.h>

//********************************************************************

#include <jlj/lang/StringBuffer.h>
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
	TextMessage reply;
	queuerequestor->request(queuesession->createTextMessage(message), 500)->downcast(reply);
	StringBuffer buf(reply->getText());
	buf->reverse();
	cout << buf << endl;
}

//********************************************************************

NAMESPACE_END(jmstools)

#endif

// eof