// Dispatcher.h

//********************************************************************

#ifndef jmstools_implementation_reactor_Dispatcher_h
#define jmstools_implementation_reactor_Dispatcher_h

//********************************************************************

#include <JMSreactorimplementation.h>
#include <jlj/util/concurrent/Callable.h>
#include <jlj/util/concurrent/ExecutorService.h>
using namespace jlj::util::concurrent;

#include <jlj/nio/reactor/Reactor.h>
using namespace jlj::nio::reactor;

#include <jlj/util/logging/Logger.h>
using namespace jlj::util;

#include <jlj/jms/Exceptions.h>

#include <jmstools/implementation/reactor/QueueFillers.h>
#include <jmstools/implementation/reactor/TransportationLayer.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation DispatcherI
	: public RunnableI
{
protected:
	~DispatcherI();
	friend class TypedReference<DispatcherI>;

public:
	DispatcherI(ExecutorService<jlj::jms::Message>, TransportationLayer, QueueFillers, BlockingQueue<int>, int dispatchertimeout);

private:
	void run();
	void dispatch() throw (Exception);
	int determineType(int);

private:
	ExecutorService<jlj::jms::Message> threadpool;
	TransportationLayer transportationlayer; 
	QueueFillers queuefillers;
	BlockingQueue<int> dispatchingstopped;
	Logger logger;
	int dispatchertimeout; 
	int polled;
	int handled;
	int dropped;

private:
	enum STATES
	{ 
		INITIAL = 0,
		POLLING, 
		TIMEOUT, 
		CLEANED,
		RECEIVED, 
		HANDLED,
		DROPPED,
		ABORTED,
		FINAL,
		IOEXCEPTION,
		TYPE_DETERMINED,
		NULL_RECEIVED, 
		CONTROL_RECEIVED,
		QUEUE_RECEIVED,
		TOPIC_RECEIVED,
	}; 	
};

//********************************************************************

StrongReferenceCommonDeclaration(Dispatcher, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof