// MessageProducer.h

//********************************************************************

#ifndef jlj_jms_MessageProducer_h
#define jlj_jms_MessageProducer_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/jms/Message.h>

using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms MessageProducerI 
	: public ObjectI
{
public:
	virtual ~MessageProducerI() = 0;

public:
	virtual void send(Message message) throw (JMSException) = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(MessageProducer, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
