//Dispatcher.h

//********************************************************************

#ifndef jmstools_jmsprovider_Dispatcher_h
#define jmstools_jmsprovider_Dispatcher_h

//********************************************************************

#include <JMSProvider.h>
#include <jlj/util/concurrent/Callable.h>
#include <jlj/util/concurrent/ExecutorService.h>
#include <jlj/nio/reactor/Reactor.h>

using namespace jlj::util::concurrent;
using namespace jlj::nio::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSProvider DispatcherI
	: public RunnableI

{
protected:
	~DispatcherI();
	friend class TypedReference<DispatcherI>;

public:
	DispatcherI(
		int minimumpoolsize,
		int maximumpoolsize,
		int keepalivetime,
		int reactortimeout,
		int acceptortimeout,
		int dispatchertimeout,
		const String& providerhost,
		int providerport) throw (jlj::lang::Exception);

public:
	void startDispatching() throw (jlj::lang::Exception);
	void runReactor();
	void stopReactor();
	void stopDispatching(int);

private:
	void run();
	void dispatch() throw (jlj::lang::Exception);
	int determineType(int);

private:
	enum STATES
	{ 
		INITIAL = 0, 
		POLLING, 
		TIMEOUT, 
		CLEANED,
		RECEIVED, 
		HANDLED,
		ABORTED,
		FINAL,
		TYPE_DETERMINED,
		INTERNAL_RECEIVED, 
		NULL_RECEIVED, 
		CONTROL_RECEIVED,
		PERSISTANT_QUEUE_RECEIVED,
		TEMPORARY_QUEUE_RECEIVED,
		PERSISTANT_TOPIC_RECEIVED,
		TEMPORARY_TOPIC_RECEIVED,
		BAD_CONNECTION,
		GOOD_CONNECTION,
		BAD_SOURCE_CONNECTION,
		GOOD_SOURCE_CONNECTION,
		BAD_DESTINATION_CONNECTION,
		GOOD_DESTINATION_CONNECTION,
		DROPPED
	}; 	

private:
	BlockingQueue<int> dispatchingstopped;
	ExecutorService<int> threadpool;
	Reactor reactor;
	int dispatchertimeout; 
	Logger logger;

};

//********************************************************************

StrongReferenceCommonDeclaration(Dispatcher, ExportedByJMSProvider);

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
