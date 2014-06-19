// QueueSession.h

//********************************************************************

#ifndef jlj_jms_QueueSession_h
#define jlj_jms_QueueSession_h

//********************************************************************

#include <jlj/jms/Session.h>
#include <jlj/jms/Queue.h>
#include <jlj/jms/QueueReceiver.h>
#include <jlj/jms/QueueSender.h>
#include <jlj/jms/QueueRequestor.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms QueueSessionI
	: public SessionI
{
public:
	virtual ~QueueSessionI() = 0;
	
public:
	virtual QueueReceiver createReceiver(Queue) throw (JMSException) = 0;
	virtual QueueSender createSender(Queue) throw (JMSException) = 0;
	virtual QueueRequestor createQueueRequestor(Queue) throw (JMSException) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(QueueSession, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
