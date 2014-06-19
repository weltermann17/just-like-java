// ServicesProvider.cpp

//********************************************************************

#ifndef ServicesProvider_cpp
#define ServicesProvider_cpp

//********************************************************************

#include <jmstools/util/services/ServicesProvider.h>
#include <jmstools/util/services/ServiceHandler.h>

//********************************************************************

#include <jlj/util/Collections.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/LinkedHashSet.h>
#include <jlj/util/StopWatch.h>
#include <jlj/util/Date.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/util/logging/StreamHandler.h>
using namespace jlj::util::logging;
#include <jlj/util/concurrent/ThreadPoolExecutor.h>
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/NullAnything.h>
using namespace jlj::util;
#include <jlj/jms/TextMessage.h>
#include <jlj/jms/MessageProducer.h>
#include <jlj/jms/QueueConnectionFactory.h>
using namespace jlj::jms;
#include <jlj/naming/InitialContext.h>
using namespace jlj::naming;
#include <signal.h>
#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

ClassLoaderInformationDefinition(jmstools::util::services::ServicesProvider, ca6e2465365413b6a3840a3f0744a081)

//********************************************************************

const int SHORTPOLLING = 20;

//********************************************************************

ServicesProviderI::~ServicesProviderI()	throw (Exception)
{
	shutdown();
	logger->log(Level::SEVERE, L"ServicesProviderI::~ServicesProviderI()");
}

//********************************************************************

ServicesProviderI::ServicesProviderI() throw (Exception)
: logger(Logger::getLogger(L"servicesprovider_logger"))
, isrunning(false)
, isshutdown(false)
, ispaused(false)
, isasynchronous(false)
, servicetime(0)
, pollingtime(0)
{
	struct StartupMonitorI : virtual public RunnableI 
	{
		StartupMonitorI(Logger logger, AtomicBoolean isrunning) 
			: logger(logger)
			, isrunning(isrunning)
			, graceperiod(60) 
		{}

		void run() 
		{
			bool monitoring = true;
			while (monitoring) 
			{
				Thread::sleep(1000);
				if (isrunning)
				{
					monitoring = false;
				}
				else 
				{
					logger->info(L"Startup monitoring: !isrunning, graceperiod : " + String(graceperiod));
					if (0 == --graceperiod) 
					{
						logger->all(L"Startup monitoring: grace period has expired. Program will be aborted now.");
						cout << flush;
						abort();
					}
				}
			}
			logger->info(L"Startup monitoring: finished successfully");
		}

		Logger logger;
		AtomicBoolean isrunning;
		int graceperiod;

	};
	static Thread thread = new ThreadI(new StartupMonitorI(logger, isrunning));
	thread->start();
}

//********************************************************************

void ServicesProviderI::setConfiguration(const String& configuration) throw (Exception)
{
	if (isrunning) shutdown();

	Anything config = 0 < configuration->length() ? Anything::decodeJSON(configuration, true) : new MapAnythingI();
	logger->info(config->toString());

	isasynchronous->set(config->containsKey(L"asynchronous") ? (bool) config->get(L"asynchronous") : false); 
	pollingtime = config->containsKey(L"pollingtime") ? (int) config->get(L"pollingtime") : 60*1000;
	busytimeout = config->containsKey(L"busytimeout") ? (int) config->get(L"busytimeout") : SHORTPOLLING;
	servicetimeout = config->containsKey(L"servicetimeout") ? (int) config->get(L"servicetimeout") : 90*1000;
	maxruntime = config->containsKey(L"maxruntime") ? (int) config->get(L"maxruntime") : 365*24*60*60;
	int maxruntimedeviation = config->containsKey(L"maxruntimedeviation") ? (int) config->get(L"maxruntimedeviation") : 0;
	if (0 < maxruntimedeviation) 
	{
		maxruntime = maxruntime - maxruntimedeviation + (rand() % (2 * maxruntimedeviation));
		logger->severe(L"maxruntime = " + String(maxruntime) + " sec");
	}
	int maxbufferingqueuesize = config->containsKey(L"maxbufferingqueuesize") ? (int) config->get(L"maxbufferingqueuesize") : 5;
	inputqueue = new LinkedBlockingQueueI<Message>(maxbufferingqueuesize);
	String queueconnectionfactoryname = config->containsKey(L"queueconnectionfactory") ? config->get(L"queueconnectionfactory")->toString() : String(L"/jms/defaultconnectionfactory");

	int minimumpoolsize = config->containsKey(L"minimumpoolsize") ? (int) config->get(L"minimumpoolsize") : 0;
	int maximumpoolsize = config->containsKey(L"maximumpoolsize") ? (int) config->get(L"maximumpoolsize") : 64;

	Map<String, QueueReceiver> q = new LinkedHashMapI<String, QueueReceiver>(); 
	receivers = Collections::synchronizedMap<String, QueueReceiver>(q);

	Map<String, ServiceImplementor> i = new LinkedHashMapI<String, ServiceImplementor>(); 
	implementors = Collections::synchronizedMap<String, ServiceImplementor>(i);

	ispaused = false;
	servicetime = 0;

	if (isasynchronous)
	{
		threadpool = new ThreadPoolExecutorI<int>(
			minimumpoolsize, 
			maximumpoolsize, 
			0, 
			new concurrent::LinkedBlockingQueueI<Runnable>());
		logger->info(L"ServicesProviderI::setConfiguration() : threadpool initialized");
	}

	signal(SIGINT, ServicesProviderI::interrupthandler);
	signal(SIGTERM, ServicesProviderI::interrupthandler);

	InitialContext initialcontext = InitialContext::newInstance();
	initialcontext->lookup(queueconnectionfactoryname, queueconnectionfactory);
	initialcontext->unbind(queueconnectionfactoryname);
	queueconnection = queueconnectionfactory->createQueueConnection();
	queuesession = queueconnection->createQueueSession();

	queueconnection->start();
}

//********************************************************************

void ServicesProviderI::registerService(const String& name, 
										ServiceImplementor service) throw (Exception)
{
	QueueReceiver queuereceiver = queuesession->createReceiver(queuesession->createQueue(name));
	queuereceiver->setMessageListener(this);
	receivers->put(name, queuereceiver);
	implementors->put(name, service);
	logger->log(Level::FINEST, "registerService() : " + name);
}

//********************************************************************

void ServicesProviderI::unregisterService(const String& name) throw (Exception)
{
	implementors->remove(name);
	receivers->remove(name);
	logger->log(Level::FINEST, "unregisterService() : " + name);
}

//********************************************************************

void ServicesProviderI::onMessage(Message message)
{
	logger->log(Level::FINEST, L"ServicesProviderI::onMessage()");

	if (!isrunning) return;
	try
	{
		if (!message) 
		{
			logger->info(L"ServicesProviderI::onMessage() : Null message received");
			stop();
			return;
		}
		if (0 < servicetime)
		{
			int now = Date::NOW()->getTime() / 1000.;
			double delta = (now - servicetime) * 1000.;
			if (servicetimeout < delta)
			{
				stop();
				serverBusy(message);
				logger->log(Level::SEVERE, L"Maximum time waiting for a service to complete exceeded: " + String(delta) + L" ms. Program will abort now.");
#if 0
				Thread::sleep(1000);
				exit(-1);
#endif
			}
		}
		if (ispaused || !isrunning || !inputqueue->offer(message, busytimeout))
		{
			pause();
			serverBusy(message);
		}
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, e->toString());
		try
		{
			MessageProducer sender = queuesession->createProducer(message->getJMSReplyTo());
			sender->send(queuesession->createTextMessage(e->toString()));
		}
		catch (const Exception& e)
		{
			logger->log(Level::SEVERE, e->toString());
		}
	}
}

//********************************************************************

void ServicesProviderI::run()
{
	struct MonitorI : virtual public RunnableI 
	{
		MonitorI(Logger logger, AtomicBoolean isrunning, AtomicBoolean isshutdown) 
			: logger(logger)
			, isrunning(isrunning)
			, isshutdown(isshutdown)
			, graceperiod(60) 
		{}

		void run() 
		{
			bool monitoring = true;
			while (monitoring) 
			{
				Thread::sleep(1000);
				if (isshutdown)
				{
					monitoring = false;
				}
				else if (!isrunning && !isshutdown)
				{
					logger->severe(L"Monitoring: !isrunning && !isshutdown, graceperiod : " + String(graceperiod));
					if (0 == --graceperiod) 
					{
						logger->all(L"Monitoring: grace period has expired. Program will be aborted now.");
						abort();
					}
				}
			}
			logger->info(L"Monitoring: finished successfully");
		}

		Logger logger;
		AtomicBoolean isrunning;
		AtomicBoolean isshutdown;
		int graceperiod;

	};
	static Thread thread = new ThreadI(new MonitorI(logger, isrunning, isshutdown)); 
	thread->start();

	if (0 == implementors->size() || 0 == receivers->size()) return;

	Message message;
	try
	{
		int runningtime = 0;
		StopWatch stopwatch = new StopWatchI();
		runninginstance() = this;
		isrunning = true;
		while (isrunning || !inputqueue->isEmpty() || 0 < ServiceHandlerI::getCountRunning())
		{
			stopwatch->start();
			message = inputqueue->poll(pollingtime);        
			logger->log(Level::FINEST, L"ServicesProviderI::run() : " + String(isrunning->get()) + L" " + String(ServiceHandlerI::getCountRunning()) + L" " + String(pollingtime));
			if (!!message)
			{
				try
				{
					int now = Date::NOW()->getTime() / 1000.; 
					servicetime = now;

					TextMessage textmessage;
					message->downcast(textmessage);

					jlj::jms::Queue queue;
					textmessage->getJMSDestination()->downcast(queue);
					ServiceImplementor service = implementors->get(queue->getQueueName());

					Anything inparameters = new MapAnythingI(); 
					Iterator<String> i = textmessage->getPropertyNames()->iterator();
					while (i->hasNext()) 
					{ 
						String name = i->next();
						inparameters->put(name, textmessage->getStringProperty(name));
					}
					service->setInParameters(inparameters);

					if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, L"ServicesProviderI::run() : " + textmessage->getText());

					if (isasynchronous) 
					{
						threadpool->execute(new ServiceHandlerI(textmessage, service, queuesession, logger));
					}
					else
					{
						Destination replyto = textmessage->getJMSReplyTo();
						if (!replyto)
						{
							service->doService(textmessage->getText());
						}
						else
						{
							String replytext = service->doService(textmessage->getText());
							if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, L"ServicesProviderI::run() replytext: " + replytext);
							TextMessage reply = queuesession->createTextMessage(replytext);

							Iterator<MapEntry<String, Anything> > i = service->getOutParameters()->entrySet()->iterator();
							while (i->hasNext())
							{
								MapEntry<String, Anything> current = i->next();
								reply->setStringProperty(current->key, current->value);
							}

							MessageProducer sender = queuesession->createProducer(replyto);
							sender->send(reply);
						}
					}
					if (ispaused && inputqueue->isEmpty()) resume();

					servicetime = 0;
				}
				catch (const Exception& e)
				{
					logger->log(Level::SEVERE, e->toString());
					if (!!message)
					{
						try
						{
							MessageProducer sender = queuesession->createProducer(message->getJMSReplyTo());
							sender->send(queuesession->createTextMessage(e->toString()));
						}
						catch (const Exception& e)
						{
							logger->log(Level::SEVERE, e->toString());
						}
					}
				}
			}
			stopwatch->stop();
			runningtime += stopwatch->getSeconds();
			if (runningtime >= maxruntime) 
			{
				pollingtime = SHORTPOLLING;
				pause(); 
				if (inputqueue->isEmpty())
				{
					isrunning = false;
					logger->info(L"Maximal runtime exceeded, servicesprovider stopped after " + String(runningtime) + L" sec");
				}
			}
		}
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, e->toString());
		if (!!message)
		{
			try
			{
				MessageProducer sender = queuesession->createProducer(message->getJMSReplyTo());
				sender->send(queuesession->createTextMessage(e->toString()));
			}
			catch (const Exception& e)
			{
				logger->log(Level::SEVERE, e->toString());
			}
		}
	}
	shutdown();
	logger->log(Level::FINEST, L"ServicesProviderI::run() : exit");
}

//********************************************************************

void ServicesProviderI::stop()
{
	if (!isrunning) return;
	try
	{
		pollingtime = SHORTPOLLING;
		isrunning = false;
		Message message;
		inputqueue->put(message);
		while (!isshutdown) 
		{
			Thread::sleep(100);
		}
		logger->log(Level::INFO, L"ServicesProviderI::stop() : stopped");
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, e->toString());
	}
}

//********************************************************************

void ServicesProviderI::pause() throw (Exception)
{
	if (ispaused) return;
	ispaused = true;
	Set<String> names = new LinkedHashSetI<String>(receivers->keySet());
	Iterator<String> i = names->iterator();
	while (i->hasNext())
	{
		try
		{
			String name = i->next();
			QueueReceiver current = receivers->get(name);
			QueueReceiver null;
			receivers->put(name, null);
		}
		catch (const Exception& e)
		{
			logger->log(Level::SEVERE, e->toString());
		}
	}
	logger->log(Level::FINEST, L"ServicesProviderI::pause()");
}

//********************************************************************

void ServicesProviderI::resume() throw (Exception)
{
	if (!ispaused || !isrunning || SHORTPOLLING == pollingtime) return;
	MessageListener null;
	Set<String> names = new LinkedHashSetI<String>(receivers->keySet());
	Iterator<String> i = names->iterator();
	while (i->hasNext())
	{
		try
		{
			String name = i->next();
			QueueReceiver queuereceiver = queuesession->createReceiver(queuesession->createQueue(name));
			queuereceiver->setMessageListener(this);
			receivers->put(name, queuereceiver);
		}
		catch (const Exception& e)
		{
			logger->log(Level::SEVERE, e->toString());
		}
	}
	ispaused = false;
	logger->log(Level::FINEST, L"ServicesProviderI::resume()");
}

//********************************************************************

void ServicesProviderI::shutdown() throw (Exception)
{
	if (!queueconnection) return;
	pause();
	receivers->clear();
	implementors->clear();
	queuesession.reset(0);
	queueconnection->stop();
	queueconnection.reset(0);
	queueconnectionfactory.reset(0);
	isshutdown = true;
	logger->log(Level::FINEST, L"ServicesProviderI::shutdown()");
}

//********************************************************************

void ServicesProviderI::serverBusy(Message message) throw (Exception)
{
	MessageProducer sender = queuesession->createProducer(message->getJMSReplyTo());
	sender->send(queuesession->createTextMessage(L"500 : Server too busy."));
}

//********************************************************************

WeakReference<ServicesProviderI>& ServicesProviderI::runninginstance()
{
	static WeakReference<ServicesProviderI> runninginstance_;
	return runninginstance_;
}

//********************************************************************

void ServicesProviderI::interrupthandler(int sig)
{
	signal(SIGINT, 0);
	signal(SIGTERM, 0);

	cout << endl << endl << "ServicesProviderI::interrupthandler() : signal = " << sig << ", program will abort" << endl << flush;
	try
	{
		Message nullmessage;
		runninginstance()->onMessage(nullmessage);
	}
	catch (const Exception&)
	{}
}

//********************************************************************

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
