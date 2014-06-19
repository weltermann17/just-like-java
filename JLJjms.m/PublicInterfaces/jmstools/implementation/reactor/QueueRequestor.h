// QueueRequestor.h

//********************************************************************

#ifndef jmstools_implementation_reactor_QueueRequestor_h
#define jmstools_implementation_reactor_QueueRequestor_h

//********************************************************************

#include <jlj/jms/QueueRequestor.h>
#include <jlj/jms/QueueSession.h>
#include <jlj/jms/Queue.h>
#include <jmstools/implementation/reactor/QueueSender.h>
#include <jmstools/implementation/reactor/QueueReceiver.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueRequestorI
	: public jlj::jms::QueueRequestorI
{
protected:
	~QueueRequestorI();
	friend class TypedReference<QueueRequestorI>;

public:
	QueueRequestorI(jlj::jms::QueueSession, jlj::jms::Queue) throw (jlj::jms::JMSException);

public:
	jlj::jms::Message request(jlj::jms::Message) throw (jlj::jms::JMSException);
	jlj::jms::Message request(jlj::jms::Message, int timeout) throw (jlj::jms::JMSException);

private:
	jlj::jms::TemporaryQueue temporaryqueue;
	jlj::jms::QueueSender sender;
	jlj::jms::QueueReceiver receiver;
};

//********************************************************************

StrongReferenceCommonDeclaration(QueueRequestor, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
