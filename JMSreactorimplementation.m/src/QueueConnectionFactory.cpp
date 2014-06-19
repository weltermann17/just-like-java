// QueueConnectionFactory.cpp

//********************************************************************

#ifndef QueueConnectionFactory_cpp
#define QueueConnectionFactory_cpp

//********************************************************************

#include <jmstools/implementation/reactor/QueueConnectionFactory.h>
#include <jmstools/implementation/reactor/QueueConnection.h>
#include <jmstools/implementation/reactor/Dispatcher.h>
#include <jmstools/implementation/reactor/ReactorTransportationLayer.h>
#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/util/logging/StreamHandler.h>
#include <jlj/util/MapAnything.h>
#include <jlj/io/FileOutputStream.h>
#include <jlj/net/InetAddress.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

ClassLoaderInformationDefinition(jmstools::implementation::reactor::QueueConnectionFactory, 87ac294a06c89922509099b6d9a386db) 

//********************************************************************

QueueConnectionFactoryI::~QueueConnectionFactoryI()
{
	if (-1001 != dispatchertimeout) // initialized
	{
		transportationlayer->finalize();
		dispatchingstopped->poll(dispatchertimeout);
		transportationlayer.reset(0);
	}
	queuefillers.reset(0);
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueConnectionFactoryI::~QueueConnectionFactoryI()");
}

//********************************************************************

QueueConnectionFactoryI::QueueConnectionFactoryI() 
: queuefillers(new QueueFillersI())
, dispatchingstopped(new LinkedBlockingQueueI<int>())
, dispatchertimeout(-1001)	// not yet initialized
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueConnectionFactoryI::QueueConnectionFactoryI()");
}

//********************************************************************

void QueueConnectionFactoryI::setConfiguration(const String& configuration) throw (jlj::jms::JMSException)
{
	Logger logger = Logger::getLogger(jlj::jms::JMS_LOGGER_NAME); 

	if (-1001 != dispatchertimeout) return;	// already initialized

	Anything config = 0 < configuration->length() ? Anything::decodeJSON(configuration, true) : new MapAnythingI();

	if (logger->isLoggable(Level::INFO)) logger->log(Level::INFO, L"QueueConnectionFactoryI::setConfiguration() : " + config->toJSON());

	dispatchertimeout = config->containsKey(L"dispatchertimeout") ? (int) config->get(L"dispatchertimeout") : 15000;
	int minimumpoolsize = config->containsKey(L"minimumpoolsize") ? (int) config->get(L"minimumpoolsize") : 8;
	int maximumpoolsize = config->containsKey(L"maximumpoolsize") ? (int) config->get(L"maximumpoolsize") : 64;
	int keepalivetime = config->containsKey(L"keepalivetime") ? (int) config->get(L"keepalivetime") : 15000;
	String providerhost = config->containsKey(L"providerhost") ? config->get(L"providerhost")->toString() : String(L"localhost");
	int providerport = config->containsKey(L"providerport") ? (int) config->get(L"providerport") : 14014;
	int reactortimeout = config->containsKey(L"reactortimeout") ? (int) config->get(L"reactortimeout") : 15000;
	
	ExecutorService<int> threadpoolint = new ThreadPoolExecutorI<int>(
		minimumpoolsize,
		maximumpoolsize,
		keepalivetime,
		new LinkedBlockingQueueI<Runnable>());

	transportationlayer = new ReactorTransportationLayerI(
		threadpoolint,
		providerhost,	
		providerport,	
		dispatchertimeout,	
		reactortimeout);	

	transportationlayer->initialize();

	ExecutorService<jlj::jms::Message> threadpoolmessage = new ThreadPoolExecutorI<jlj::jms::Message>(
		minimumpoolsize,
		maximumpoolsize,
		keepalivetime,
		new LinkedBlockingQueueI<Runnable>());
	
	Dispatcher dispatcher = new DispatcherI(
		threadpoolmessage,
		transportationlayer, 
		queuefillers,
		dispatchingstopped, 
		dispatchertimeout);
	
	threadpoolint->execute(dispatcher);
}

//********************************************************************

jlj::jms::Connection QueueConnectionFactoryI::createConnection() throw (jlj::jms::JMSException)
{
	return createQueueConnection();
}

//********************************************************************

jlj::jms::QueueConnection QueueConnectionFactoryI::createQueueConnection() throw (jlj::jms::JMSException)
{
	if (-1001 == dispatchertimeout) 
		throw jlj::jms::JMSException(WITHDETAILS(L"QueueConnectionFactory not yet initialized. setConfiguration() was not called."));
	return new QueueConnectionI(transportationlayer, queuefillers);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
