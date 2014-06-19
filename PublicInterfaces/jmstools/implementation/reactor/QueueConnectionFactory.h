// QueueConnectionFactory.h

//********************************************************************

#ifndef jmstools_implementation_reactor_QueueConnectionFactory_h
#define jmstools_implementation_reactor_QueueConnectionFactory_h

//********************************************************************

#include <jlj/jms/QueueConnectionFactory.h>
#include <jlj/jms/QueueConnection.h>
#include <jmstools/implementation/reactor/TransportationLayer.h>
#include <jmstools/implementation/reactor/QueueFillers.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueConnectionFactoryI
	: public jlj::jms::QueueConnectionFactoryI
{
protected:
	~QueueConnectionFactoryI();
	friend class TypedReference<QueueConnectionFactoryI>;

public:
	QueueConnectionFactoryI();

public:
	jlj::jms::Connection createConnection() throw (jlj::jms::JMSException);
	jlj::jms::QueueConnection createQueueConnection() throw (jlj::jms::JMSException);
	void setConfiguration(const String& configuration) throw (jlj::jms::JMSException);

private:
	TransportationLayer transportationlayer;
	BlockingQueue<int> dispatchingstopped;
	int dispatchertimeout;
	QueueFillers queuefillers;

};

//********************************************************************

StrongReferenceCommonDeclaration(QueueConnectionFactory, ExportedByJMSreactorimplementation);
ClassLoaderInformationDeclaration(jmstools::implementation::reactor::QueueConnectionFactory, 87ac294a06c89922509099b6d9a386db, ExportedByJMSreactorimplementation) 

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
