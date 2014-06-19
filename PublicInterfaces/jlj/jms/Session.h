// Session.h

//********************************************************************

#ifndef jlj_jms_Session_h
#define jlj_jms_Session_h

//********************************************************************

#include <jlj/jms/TemporaryQueue.h>
#include <jlj/jms/MessageConsumer.h>
#include <jlj/jms/MessageProducer.h>
#include <jlj/jms/AnythingMessage.h>
#include <jlj/jms/TextMessage.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms SessionI 
	: public ObjectI
{
public:
	~SessionI() = 0;
	
	virtual MessageProducer createProducer(Destination) throw (JMSException) = 0;
	virtual MessageConsumer createConsumer(Destination) throw (JMSException) = 0;
	
	virtual Queue createQueue(const String&) throw (JMSException) = 0;
	virtual TemporaryQueue createTemporaryQueue() throw (JMSException) = 0;
	
	virtual AnythingMessage createAnythingMessage(Anything) throw (JMSException) = 0;
	virtual TextMessage createTextMessage(const String&) throw (JMSException) = 0;

};

//*********************************************************************

StrongReferenceCommonDeclaration(Session, ExportedByJLJjms);

//*********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************
#endif
// eof
