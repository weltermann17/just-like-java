// QueueSender.h

//*********************************************************************

#ifndef jlj_jms_QueueSender_h
#define jlj_jms_QueueSender_h

//*********************************************************************

#include <jlj/jms/MessageProducer.h>

//*********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//*********************************************************************

class ExportedByJLJjms QueueSenderI
	: public MessageProducerI
{
public:
	virtual ~QueueSenderI() = 0;
};

//*********************************************************************

StrongReferenceCommonDeclaration(QueueSender, ExportedByJLJjms);

//*********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//*********************************************************************

#endif

// eof
