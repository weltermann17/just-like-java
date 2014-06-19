// Queue.h

//********************************************************************

#ifndef jlj_jms_Queue_h
#define jlj_jms_Queue_h

//********************************************************************

#include <jlj/jms/Destination.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms QueueI
	: public virtual DestinationI
{
public:
	virtual ~QueueI() = 0;
	
public:
	virtual String getQueueName() const = 0;
	virtual String toString() const = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(Queue, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
