//QueueFillers.h

//********************************************************************

#ifndef jmstools_implementation_reactor_QueueFillers_h
#define jmstools_implementation_reactor_QueueFillers_h

//********************************************************************

#include <JMSreactorimplementation.h>
#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>

#include <jlj/util/Collections.h>

#include <jmstools/implementation/reactor/QueueFiller.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/jms/Message.h>

#include <jlj/jms/Exceptions.h>

using namespace jlj::lang;
using namespace jlj::util;
#include <jlj/util/concurrent/ThreadPoolExecutor.h>
using namespace jlj::util::concurrent;

#include <jlj/util/concurrent/atomic/AtomicInteger.h>
using namespace jlj::util::concurrent::atomic;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation QueueFillersI
	: public ObjectI
{
public:	
	QueueFillersI();

public:
	void addQueueFiller(const String& queuename, QueueFiller);
	void removeQueueFiller(const String& queuename, QueueFiller);
	void broadcast(ExecutorService<jlj::jms::Message>, Message) throw (Exception);
	void broadcastAbort(ExecutorService<jlj::jms::Message>, int timeout) throw (Exception);
	void broadcastToExpired(ExecutorService<jlj::jms::Message>, Message) throw (Exception);
	QueueFiller getQueueFiller(const String&) throw (Exception);

protected:
   	~QueueFillersI();
    friend class TypedReference<QueueFillersI>;

private:
	SynchronizedMap<String, SynchronizedList<QueueFiller> > queuefillers;
	AtomicInteger roundrobin;

};

//********************************************************************

StrongReferenceCommonDeclaration(QueueFillers, ExportedByJMSreactorimplementation);


//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
