// QueueReceiver.h

//********************************************************************

#ifndef jlj_jms_QueueReceiver_h
#define jlj_jms_QueueReceiver_h

//********************************************************************

#include <jlj/jms/MessageConsumer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms QueueReceiverI
	: public MessageConsumerI
{
public:
	virtual ~QueueReceiverI() = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(QueueReceiver, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
