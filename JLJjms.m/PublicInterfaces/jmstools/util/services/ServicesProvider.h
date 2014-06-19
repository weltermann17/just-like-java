// ServicesProvider.h

//********************************************************************

#ifndef jmstools_util_services_ServicesProvider_h
#define jmstools_util_services_ServicesProvider_h

//********************************************************************

#include <jmstools/util/services/ServiceImplementor.h>

//********************************************************************

#include <jlj/lang/Runnable.h>
#include <jlj/util/AbstractNamedSingleton.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/MapAnything.h>
using namespace jlj::util;
#include <jlj/util/concurrent/ExecutorService.h>
#include <jlj/util/concurrent/BlockingQueue.h>
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
using namespace jlj::util::concurrent;
using namespace jlj::util::concurrent::atomic;
#include <jlj/jms/MessageListener.h>
#include <jlj/jms/QueueReceiver.h>
#include <jlj/jms/QueueSession.h>
#include <jlj/jms/QueueConnectionFactory.h>
using namespace jlj::jms;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

/**
* if it is not possible to queue a request within busytimeout the provider will pause 
* if a serviceimplementor does not reply within servicetimeout the provider will pause
* after 2x the servicetimeout the provider will abort the hosting program
* stop running after maxruntime (all timeouts in ms)
* should be small (1-10)
*/

class ExportedByJMSTOOLSutil ServicesProviderI
	: public virtual jlj::jms::MessageListenerI
	, public virtual jlj::lang::RunnableI
{
protected:
	~ServicesProviderI() throw (Exception);
	friend class TypedReference<ServicesProviderI>;

public:
	ServicesProviderI() throw (Exception);
	
	virtual void setConfiguration(const String& configuration) throw (Exception);

	virtual void registerService(const String& name, jmstools::util::services::ServiceImplementor) throw (Exception);
	virtual void unregisterService(const String& name) throw (Exception);

	void run();
	virtual void stop();

private:

	void onMessage(Message);

	void pause() throw (Exception);
	void resume() throw (Exception);
	void shutdown() throw (Exception);
	void serverBusy(Message) throw (Exception);

	static WeakReference<ServicesProviderI>& runninginstance();
	static void interrupthandler(int);

	Map<String, QueueReceiver> receivers;
	Map<String, jmstools::util::services::ServiceImplementor> implementors;
	BlockingQueue<Message> inputqueue;
	QueueConnectionFactory queueconnectionfactory;
	QueueConnection queueconnection;
	QueueSession queuesession;
	int busytimeout;
	int servicetimeout;
	int maxruntime;
	Logger logger;
	AtomicBoolean isrunning;
	AtomicBoolean isshutdown;
	AtomicInteger pollingtime;
	AtomicBoolean ispaused;
	AtomicInteger servicetime;
	AtomicBoolean isasynchronous;
	ExecutorService<int> threadpool;

};

//********************************************************************

ClassLoaderInformationDeclaration(jmstools::util::services::ServicesProvider, ca6e2465365413b6a3840a3f0744a081, ExportedByJMSTOOLSutil); 
StrongReferenceCommonDeclaration(ServicesProvider, ExportedByJMSTOOLSutil);

//********************************************************************

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
