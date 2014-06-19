// MessageConsumer.h

//********************************************************************

#ifndef jlj_jms_MessageConsumer_h
#define jlj_jms_MessageConsumer_h

//********************************************************************

#include <jlj/jms/Message.h>
#include <jlj/jms/MessageListener.h>
#include <jlj/jms/Exceptions.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms MessageConsumerI 
	: public ObjectI
{
public:
	virtual ~MessageConsumerI() = 0;
	
public:
	virtual Message	receive() throw (JMSException) = 0;
	virtual Message	receive(int timeout) throw (JMSException) = 0;
	virtual void setMessageListener(MessageListener listener) throw (JMSException) = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(MessageConsumer, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************
#endif

// eof
