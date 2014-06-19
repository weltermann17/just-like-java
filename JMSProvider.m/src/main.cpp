// main.cpp

//**********************************************************************

#include <JMSProvider.h>
#include <Dispatcher.h>
#include <jlj/lifetime/JLJLifetimeImplementation.h>
#include <jlj/io/FileOutputStream.h>
#include <jlj/util/Uuid.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/logging/ConsoleHandler.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/naming/InitialContext.h>
#include <jlj/net/InetAddress.h>
#include <jlj/lang/Thread.h>
using namespace jmstools;
using namespace jlj::io;
using namespace jlj::util;
using namespace jlj::util::logging;
using namespace jlj::lifetime;
using namespace jmstools::implementation::reactor;
using namespace jlj::naming;
using namespace jlj::net;
using namespace jlj::lang;

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <iostream>
using namespace std;

//**********************************************************************

Dispatcher dispatcherinstance;

//**********************************************************************

void interruptreactor(int)
{
	signal(SIGINT, 0);
	dispatcherinstance->stopReactor();
}

//**********************************************************************

int main(int argc, char** argv) 
{
	const String defaultconfiguration = 
		L"{\n" \
		L"    \"java.naming.factory.initial\": {\n" \
		L"        \"jms\" : {\n" \
		L"            \"class\" : \"jmstools::naming::JMSInitialContext\",\n" \
		L"            \"library\" : \"JMSTOOLSutil\",\n" \
		L"            \"configuration\" : {\n" \
		L"              \"jms\" : {\n" \
		L"                  \"jmslogger\" : {\n" \
		L"                      \"name\" : \"jms_logger\",\n" \
		L"                      \"level\" : \"off\"\n" \
		L"                  },\n" \
		L"                  \"reactor\" : {\n" \
		L"                      \"defaultprovider\" : \"/jms/reactor/provider1\",\n" \
		L"                      \"provider1\" : { \"providerhost\" : \"${env/jms_provider_host}\", \"providerport\" : \"${env/jms_provider_port}\" },\n" \
		L"                      \"reactorlogger\" : {\n" \
		L"                          \"name\" : \"reactor_logger\",\n" \
		L"                          \"level\" : \"off\"\n" \
		L"                      }\n" \
		L"                  }\n" \
		L"               }\n" \
		L"            }\n" \
		L"        }\n" \
		L"    },\n" \
		L"    \"logger\" : {\n" \
		L"        \"name\" : \"global_logger\",\n" \
		L"        \"level\" : \"off\"\n" \
		L"    }\n" \
		L"}\n";

	try
	{		
		LifetimeImplementation jljlifetime = new JLJLifetimeImplementationI(defaultconfiguration);
		jljlifetime->initialize();
		Logger logger = Logger::getLogger(jlj::jms::JMS_LOGGER_NAME);
		try
		{
			Anything config = new NullAnythingI();
			InitialContext initialcontext = InitialContext::newInstance();
			if (initialcontext->isBound(L"/jms/reactor/defaultprovider")) 
			{
				initialcontext->lookup(L"/jms/reactor/defaultprovider", config);
			}

			int minimumpoolsize = config->containsKey(L"minimumpoolsize") ? (int) config->get(L"minimumpoolsize") : 32;
			int maximumpoolsize = config->containsKey(L"maximumpoolsize") ? (int) config->get(L"maximumpoolsize") : 128;
			int keepalivetime = config->containsKey(L"keepalivetime") ? (int) config->get(L"keepalivetime") : 15000;
			int reactortimeout = config->containsKey(L"reactortimeout") ? (int) config->get(L"reactortimeout") : 15000;
			int acceptortimeout = config->containsKey(L"acceptortimeout") ? (int) config->get(L"acceptortimeout") : 15000;
			int dispatchertimeout = config->containsKey(L"dispatchertimeout") ? (int) config->get(L"dispatchertimeout") : 15000;
			String providerhost = config->containsKey(L"providerhost") ? config->get(L"providerhost")->toString() : InetAddress::getLocalHost()->getCanonicalHostName();
			int providerport = config->containsKey(L"providerport") ? (int) config->get(L"providerport") : 14014;

			Dispatcher dispatcher = new DispatcherI(
				minimumpoolsize,
				maximumpoolsize,
				keepalivetime,
				reactortimeout,
				acceptortimeout, 
				dispatchertimeout, 
				providerhost, 
				providerport);

			dispatcherinstance = dispatcher;
			signal(SIGINT, interruptreactor);
			dispatcher->startDispatching();
			dispatcher->runReactor();
			signal(SIGINT, 0);
			dispatcherinstance.reset(0);
			dispatcher->stopDispatching(dispatchertimeout);

			logger->log(Level::SEVERE, L"JMSProvider : Dispatcher stopped."); 
		}
		catch (const Exception& e)
		{
			logger->addHandler(ConsoleHandler::getInstance());
			logger->setLevel(Level::SEVERE);
			logger->log(Level::SEVERE, L"JMSProvider : " + e->toString()); 
		}
		catch (...)
		{
			logger->log(Level::SEVERE, L"JMSProvider : Unknown exception (inner)."); 
		}

		jljlifetime->finalize();
	}
	catch (const Exception& e)
	{
		fprintf(stderr, "JMSProvider : %s\n", constcharptr(e->toString())); 
	}
	catch (...)
	{
		fprintf(stderr, "JMSProvider : Unknown exception (outer).\n"); 
	}

#if defined(_MSC_VER) 
	return 0;
#else 
	_Exit(0);
#endif
}

//**********************************************************************

// eof
