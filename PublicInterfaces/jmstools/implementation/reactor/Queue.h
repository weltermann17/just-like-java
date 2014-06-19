// Queue.h

//********************************************************************

#ifndef jmstools_implementation_reactor_Queue_h
#define jmstools_implementation_reactor_Queue_h

//********************************************************************

#include <JMSreactorimplementation.h>
#include <jlj/jms/Queue.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueI
	: public virtual jlj::jms::QueueI
{
protected:	
	~QueueI();
	friend class TypedReference<QueueI>;

public:
	QueueI(const String& name, int type, const String& identifier, bool jsonencoding);
	QueueI(const String& name, int type);
	QueueI(const String& name);

public:
	String getQueueName() const;
	String toString() const;
	int getType() const;
	String getIdentifier() const;
	bool getJsonEncoding() const;

private: 
	String name;
	int type;
	String identifier;
	bool jsonencoding;
};

//********************************************************************

struct ExportedByJMSreactorimplementation Queue : StrongReference<QueueI>
{
	StrongReferenceCommonDerivateImplementation(Queue);
};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
