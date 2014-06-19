// Message.h

//********************************************************************

#ifndef jlj_jms_Message_h
#define jlj_jms_Message_h

//********************************************************************

#include <jlj/jms/Destination.h>
#include <jlj/lang/String.h>
#include <jlj/util/Anything.h>

using namespace jlj::lang;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms MessageI 
	: public ObjectI
{
public:
	virtual ~MessageI() = 0;

public:
	virtual Destination getJMSDestination() const throw (JMSException) = 0;
	virtual String getJMSMessageID() const throw (JMSException) = 0;
	virtual Destination getJMSReplyTo() const throw (JMSException) = 0;
	virtual double getJMSTimestamp() const throw (JMSException) = 0;
	virtual void setJMSReplyTo(Destination) throw (JMSException) = 0;
	virtual void setJMSDestination(Destination) throw (JMSException) = 0;

	virtual String getStringProperty(const String& name) const throw (JMSException) = 0;
	virtual List<String> getPropertyNames() const throw (JMSException) = 0;
	virtual void setStringProperty(const String& name, const String& value) throw (JMSException) = 0;
	virtual bool propertyExists(const String& name) const throw (JMSException) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Message, ExportedByJLJjms);

extern ExportedByJLJjms const String JMS_LOGGER_NAME;

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
