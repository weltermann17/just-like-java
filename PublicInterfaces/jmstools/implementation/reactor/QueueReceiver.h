// QueueReceiver.h

//********************************************************************

#ifndef jmstools_implementation_reactor_QueueReceiver_h
#define jmstools_implementation_reactor_QueueReceiver_h

//********************************************************************

#include <jlj/jms/QueueReceiver.h>
#include <jmstools/implementation/reactor/Queue.h>
#include <jmstools/implementation/reactor/QueueFiller.h>
#include <jmstools/implementation/reactor/QueueFillers.h>
#include <jmstools/implementation/reactor/QueueSession.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueReceiverI
	: public jlj::jms::QueueReceiverI
{
protected:
	~QueueReceiverI() throw (jlj::jms::JMSException);
	friend class TypedReference<QueueReceiverI>;

public:
	QueueReceiverI(Queue, QueueSession, QueueFillers, TransportationLayer) throw (jlj::jms::JMSException);

public:
	jlj::jms::Message receive() throw (jlj::jms::JMSException);
	jlj::jms::Message receive(int) throw (jlj::jms::JMSException);
	void setMessageListener(jlj::jms::MessageListener) throw (jlj::jms::JMSException);

private:
	Queue queue;
	QueueSession session;
	QueueFiller queuefiller;
	QueueFillers queuefillers;
	TransportationLayer transportationlayer;

};

//********************************************************************

StrongReferenceCommonDeclaration(QueueReceiver, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
