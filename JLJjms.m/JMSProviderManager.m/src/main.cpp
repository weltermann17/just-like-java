// main.cpp

#include <JMSProviderManager.h>
#include <ProviderManager.h>
#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/StringAnything.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/lifetime/JLJLifetimeImplementation.h>
#include <jlj/naming/InitialContext.h>
#include <jlj/lang/Thread.h>
using namespace jlj::naming;
using namespace jlj::util::logging;
using namespace jlj::lifetime;
using namespace jmstools;

#include <iostream>
using namespace std;

//***************************************************************

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
		L"                  \"defaultconnectionfactory\" : \"/jms/reactor/queueconnectionfactory1\",\n" \
		L"                  \"jmslogger\" : {\n" \
		L"                      \"name\" : \"jms_logger\",\n" \
		L"                      \"level\" : \"off\"\n" \
		L"                  },\n" \
		L"                  \"reactor\" : {\n" \
		L"                      \"queueconnectionfactory1\" : {\n" \
		L"                          \"class\" : \"jmstools::implementation::reactor::QueueConnectionFactory\",\n" \
		L"                          \"library\" : \"JMSreactorimplementation\",\n" \
		L"                          \"configuration\" : {}\n" \
		L"                      },\n" \
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

	int rc = 0;

	try
	{
		LifetimeImplementation jljlifetime = new JLJLifetimeImplementationI(defaultconfiguration);
		jljlifetime->initialize();

		Logger logger = Logger::getLogger(jlj::jms::JMS_LOGGER_NAME);

		try
		{
			QueueConnectionFactory queueconnectionfactory;
			InitialContext initialcontext = InitialContext::newInstance();
			initialcontext->lookup(L"/jms/defaultconnectionfactory", queueconnectionfactory);	

			ProviderManager manager = new ProviderManagerI(queueconnectionfactory, logger);
			if (argc == 2)
			{
				String command(argv[1], strlen(argv[1])); 
				command = command->toLowerCase();
				manager->start(command, L"", false);
			}
			else if (argc == 3) 
			{
				String command(argv[1], strlen(argv[1])); 
				command = command->toLowerCase();
				String parameter(argv[2], strlen(argv[2]));
				manager->start(command, parameter, manager->isNagiosOn(parameter->toLowerCase()));	
			}
			else if (argc == 4) 
			{
				String command(argv[1], strlen(argv[1])); 
				command = command->toLowerCase();
				String parameter(argv[2], strlen(argv[2]));
				String nagios(argv[3], strlen(argv[3]));
				if (manager->isNagiosOn(nagios->toLowerCase())) manager->start(command, parameter, true);
				else 	manager->usage();
			}
			else
			{
				logger->log(Level::SEVERE, L"Invalid syntax:");
				manager->usage();
			}
		}
		catch (const Exception& e)
		{
			rc = 1;
			logger->addHandler(ConsoleHandler::getInstance());
			logger->setLevel(Level::SEVERE);
			logger->log(Level::SEVERE, L"Error in JMSProviderManager: "  + e->toString()); 
		}
		catch (const std::exception& e) 
		{
			rc = 2;
			logger->addHandler(ConsoleHandler::getInstance());
			logger->setLevel(Level::SEVERE);
			logger->log(Level::SEVERE, L"Error " + String(e.what())); 
		}
		catch (...)
		{
			rc = 3;
			logger->addHandler(ConsoleHandler::getInstance());
			logger->setLevel(Level::SEVERE);
			logger->log(Level::SEVERE, L"JMSProviderManager : unknown exception");
		}

		jljlifetime->finalize();
	}
	catch (Exception& e)
	{
		rc = 4;
		std::cout << e << std::endl;
	}

#if defined(_MSC_VER) 
	return rc;
#else 
	_Exit(0);
#endif
}

// eof