//QueueFiller.h

//********************************************************************

#ifndef jmstools_implementation_reactor_QueueFiller_h
#define jmstools_implementation_reactor_QueueFiller_h

//********************************************************************

#include <jlj/lang/Runnable.h>
#include <jlj/util/Anything.h>
#include <jmstools/implementation/reactor/Handler.h>
#include <jlj/jms/MessageListener.h>
#include <jlj/jms/Message.h>
#include <jlj/util/concurrent/ExecutorService.h>
#include <jlj/util/concurrent/BlockingQueue.h>
#include <jlj/util/concurrent/atomic/AtomicInteger.h>

using namespace jlj::util::concurrent::atomic;
using namespace jlj::util::concurrent;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueFillerI
	: public ObjectI
{
protected:
	~QueueFillerI();
	friend class TypedReference<QueueFillerI>;

public:
	QueueFillerI();
	QueueFillerI(jlj::jms::MessageListener);

public:
	jlj::jms::Message get(int timeout) const throw (jlj::lang::Exception);
	void put(ExecutorService<jlj::jms::Message>, Anything) throw (jlj::jms::JMSException);
	bool hasExpired() const;

private:
	jlj::jms::MessageListener listener;	
	BlockingQueue<jlj::jms::Message> queue;
	mutable AtomicInteger timeout;
	mutable AtomicInteger count;
};

//********************************************************************

StrongReferenceCommonDeclaration(QueueFiller, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
