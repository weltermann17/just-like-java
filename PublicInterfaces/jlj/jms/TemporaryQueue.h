// TemporaryQueue.h

//********************************************************************
#ifndef jlj_jms_TemporaryQueue_h
#define jlj_jms_TemporaryQueue_h

//********************************************************************

#include <jlj/jms/Queue.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms TemporaryQueueI
	: public virtual QueueI
{
public:
	virtual ~TemporaryQueueI() = 0;
};

//*********************************************************************

struct ExportedByJLJjms TemporaryQueue : StrongReference<TemporaryQueueI>
{
	StrongReferenceCommonDerivateImplementation(TemporaryQueue);

};

//*********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//*********************************************************************

#endif

// eof
