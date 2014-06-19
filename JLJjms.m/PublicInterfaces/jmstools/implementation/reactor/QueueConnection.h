// QueueConnection.h

//********************************************************************

#ifndef jmstools_implementation_reactor_QueueConnection_h
#define jmstools_implementation_reactor_QueueConnection_h

//********************************************************************

#include <jlj/jms/QueueConnection.h>
#include <jlj/jms/QueueSession.h>
#include <jlj/jms/Exceptions.h>
#include <jlj/jms/Message.h>
#include <jmstools/implementation/reactor/TransportationLayer.h>
#include <jmstools/implementation/reactor/QueueFillers.h>
#include <jlj/nio/reactor/Connection.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueConnectionI
	: public jlj::jms::QueueConnectionI
{
protected:
	~QueueConnectionI();
	friend class TypedReference<QueueConnectionI>;

public:	
	QueueConnectionI(TransportationLayer, QueueFillers) throw (jlj::jms::JMSException);

public:
	jlj::jms::Session createSession() throw (jlj::jms::JMSException);
	jlj::jms::QueueSession createQueueSession() throw (jlj::jms::JMSException);
	void close() throw (jlj::jms::JMSException);
	void start() throw (jlj::jms::JMSException);
	void stop() throw (jlj::jms::JMSException);

private:
	TransportationLayer transportationlayer;
	QueueFillers queuefillers;
};

//********************************************************************

StrongReferenceCommonDeclaration(QueueConnection, ExportedByJMSreactorimplementation);

//********************************************************************
NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
