// QueueSender.h

//********************************************************************

#ifndef jmstools_implementation_reactor_QueueSender_h
#define jmstools_implementation_reactor_QueueSender_h

//********************************************************************

#include <jlj/jms/QueueSender.h>
#include <jmstools/implementation/reactor/Queue.h>
#include <jmstools/implementation/reactor/TransportationLayer.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueSenderI
	: public jlj::jms::QueueSenderI
{
protected:
	~QueueSenderI();
	friend class TypedReference<QueueSenderI>;

public:
	QueueSenderI(Queue, TransportationLayer);

public:
	void send(jlj::jms::Message) throw (jlj::jms::JMSException);

private:
	Queue queue;
	TransportationLayer transportationlayer;
};

//********************************************************************

StrongReferenceCommonDeclaration(QueueSender, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
