// TemporaryQueue.h

//********************************************************************

#ifndef jmstools_implementation_reactor_TemporaryQueue_h
#define jmstools_implementation_reactor_TemporaryQueue_h

//********************************************************************

#include <jlj/jms/TemporaryQueue.h>
#include <jmstools/implementation/reactor/Queue.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation TemporaryQueueI
	: public jlj::jms::TemporaryQueueI
	, public jmstools::implementation::reactor::QueueI
{
protected:
	~TemporaryQueueI();
	friend class TypedReference<TemporaryQueueI>;

public:
	TemporaryQueueI(const String& name);

	String toString() const;

};

//********************************************************************

struct ExportedByJMSreactorimplementation TemporaryQueue : StrongReference<TemporaryQueueI>
{
	StrongReferenceCommonDerivateImplementation(TemporaryQueue);

};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
