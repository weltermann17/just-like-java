// QueueSession.h

//********************************************************************

#ifndef jmstools_implementation_reactor_QueueSession_h
#define jmstools_implementation_reactor_QueueSession_h

//********************************************************************

#include <jlj/jms/QueueSession.h>
#include <jmstools/implementation/reactor/TransportationLayer.h>
#include <jmstools/implementation/reactor/QueueFillers.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueSessionI
	: public jlj::jms::QueueSessionI
{
protected:
	~QueueSessionI();
	friend class TypedReference<QueueSessionI>;

public:
	QueueSessionI(TransportationLayer, QueueFillers) throw (jlj::jms::JMSException);

public:
	jlj::jms::MessageConsumer createConsumer(jlj::jms::Destination) throw (jlj::jms::JMSException);
	jlj::jms::MessageProducer createProducer(jlj::jms::Destination) throw (jlj::jms::JMSException);
	jlj::jms::AnythingMessage createAnythingMessage(jlj::util::Anything) throw (jlj::jms::JMSException);
	jlj::jms::TextMessage createTextMessage(const String&) throw (jlj::jms::JMSException);
	jlj::jms::Queue createQueue(const String&) throw (jlj::jms::JMSException);
	jlj::jms::TemporaryQueue createTemporaryQueue() throw (jlj::jms::JMSException);
	jlj::jms::QueueReceiver createReceiver(jlj::jms::Queue) throw (jlj::jms::JMSException);
	jlj::jms::QueueSender createSender(jlj::jms::Queue) throw (jlj::jms::JMSException);
	jlj::jms::QueueRequestor createQueueRequestor(jlj::jms::Queue) throw (jlj::jms::JMSException);

private:
	TransportationLayer transportationlayer;
	QueueFillers queuefillers;

};

//********************************************************************

StrongReferenceCommonDeclaration(QueueSession, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
