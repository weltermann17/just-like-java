// AbstractMessage.h

//********************************************************************

#ifndef jmstools_implementation_reactor_AbstractMessage_h
#define jmstools_implementation_reactor_AbstractMessage_h

//********************************************************************

#include <JMSreactorimplementation.h>
#include <jlj/jms/Message.h>
#include <jmstools/implementation/reactor/Message.h>

using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation AbstractMessageI
	: public virtual jlj::jms::MessageI
	, public jmstools::implementation::reactor::MessageI
{
protected:
	~AbstractMessageI();
	friend class TypedReference<AbstractMessageI>;

public:
	AbstractMessageI();
	AbstractMessageI(Anything);

public:
	jlj::jms::Destination getJMSDestination() const throw (jlj::jms::JMSException);
	String getJMSMessageID() const throw (jlj::jms::JMSException);
	jlj::jms::Destination getJMSReplyTo() const throw (jlj::jms::JMSException);
	double getJMSTimestamp() const throw (jlj::jms::JMSException);

	String getStringProperty(const String&) const throw (jlj::jms::JMSException);
	List<String> getPropertyNames() const throw (jlj::jms::JMSException);

	void setJMSReplyTo(jlj::jms::Destination) throw (jlj::jms::JMSException);
	void setJMSDestination(jlj::jms::Destination) throw (jlj::jms::JMSException);
	void setStringProperty(const String&, const String&) throw (jlj::jms::JMSException);

	bool propertyExists(const String&) const throw (jlj::jms::JMSException);

};

//********************************************************************

struct ExportedByJMSreactorimplementation AbstractMessage : StrongReference<AbstractMessageI>
{
	StrongReferenceCommonDerivateImplementation(AbstractMessage);
};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
