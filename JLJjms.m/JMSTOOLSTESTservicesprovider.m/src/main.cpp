// main.cpp

//*********************************************************************

#include <JMSTOOLSTESTservicesprovider.h>

//**********************************************************************

#include <TestServiceClient.h>
using namespace jmstools;

//**********************************************************************

#include <cppunit/ui/text/TestRunner.h>
#include <jlj/lang/ClassLoader.h>
#include <jlj/lang/Thread.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;
#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/lifetime/JLJLifetimeImplementation.h>
using namespace jlj::lifetime;
#include <jlj/util/StringAnything.h>
using namespace jlj::util;
using namespace jlj::util::logging;
#include <jlj/jms/QueueConnectionFactory.h>
using namespace jlj::jms;
#include <jlj/naming/InitialContext.h>
using namespace jlj::naming;
#include <jmstools/util/services/ServicesProvider.h>
using namespace jmstools::util::services;

#include <iostream>
using namespace std;

//**********************************************************************

int main(int argc, char** argv) 
{
	try
	{
		const String defaultconfiguration = 
			L"{\n" \
			L"    \"java.naming.factory.initial\": {\n" \
			L"        \"jms\" : {\n" \
			L"            \"class\" : \"jmstools::naming::JMSInitialContext\",\n" \
			L"            \"library\" : \"JMSTOOLSutil\",\n" \
			L"            \"load\" : true,\n" \
			L"            \"configuration\" : {\n" \
			L"              \"jms\" : {\n" \
	    	L"                  \"defaultconnectionfactory\" : \"/jms/reactor/queueconnectionfactory1\",\n" \
			L"                  \"jmslogger\" : {\n" \
			L"                      \"name\" : \"jms_logger\",\n" \
			L"                      \"level\" : \"off\"\n" \
			L"                  },\n" \
			L"                  \"reactor\" : {\n" \
			L"                      \"defaultprovider\" : \"/jms/reactor/provider1\",\n" \
			L"                      \"provider1\" : {},\n" \
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
			L"        },\n" \
			L"       \"enoviav5\" : {\n" \
			L"            \"class\" : \"enoviav5::naming::ENOVIAV5InitialContext\",\n" \
			L"            \"library\" : \"ENOVIAV5naming\",\n" \
			L"            \"load\" : false,\n" \
			L"            \"configuration\" : {},\n" \
			L"            \"enoviav5logger\" : {\n" \
			L"                \"name\" : \"enoviav5_logger\",\n" \
			L"                \"level\" : \"off\"\n" \
			L"            },\n" \
			L"            \"enoviav5eventlogger\" : {\n" \
			L"                \"name\" : \"enoviav5_event_logger\",\n"\
			L"                \"level\" : \"off\"\n" \
			L"            }\n" \
			L"        },\n" \
			L"        \"manenoviav5\" : {\n" \
			L"            \"class\" : \"manenoviav5::naming::MANENOVIAV5InitialContext\",\n" \
			L"            \"library\" : \"MANENOVIAV5naming\",\n" \
			L"            \"load\" : false,\n" \
			L"            \"configuration\" : {},\n" \
			L"            \"manenoviav5logger\" : {\n" \
			L"                \"name\" : \"manenoviav5_logger\",\n" \
			L"                \"level\" : \"off\"\n" \
			L"            }\n" \
			L"        },\n" \
			L"        \"servicesprovider\" : {\n" \
			L"            \"class\" : \"jmstools::util::services::ServicesProviderInitialContext\",\n" \
			L"            \"library\" : \"JMSTOOLSutil\",\n" \
			L"            \"load\" : false,\n" \
			L"            \"configuration\" : {},\n" \
			L"            \"servicesproviderlogger\" : {\n" \
			L"                \"name\" : \"servicesprovider_logger\",\n" \
			L"                \"level\" : \"off\"\n" \
			L"            }\n" \
			L"        }\n" \
			L"    },\n" \
			L"    \"logger\" : {\n" \
			L"        \"name\" : \"global_logger\",\n" \
			L"        \"level\" : \"severe\"\n" \
			L"    }\n" \
			L"}\n";

		LifetimeImplementation jljlifetime = new JLJLifetimeImplementationI(defaultconfiguration);
		jljlifetime->initialize();

		if (1 < argc)
		{	
			String type_client = String(argv[1], strlen(argv[1]));

			if (type_client== L"CLIENT") 
			{
				int numberofclients = atoi(argv[2]);
				int loops = atoi(argv[3]);
				cout << "starting clients : " << numberofclients << ", loops : " << loops << endl;
	
				InitialContext initialcontext = InitialContext::newInstance();
				QueueConnectionFactory queueconnectionfactory;
				initialcontext->lookup(L"/jms/defaultconnectionfactory", queueconnectionfactory);				
				QueueConnection connection = queueconnectionfactory->createQueueConnection();
				connection->start();
				QueueSession queuesession = connection->createQueueSession();

				int ccount = 0;
				int exceptioncount = 0;
				for (int i = 0; i < loops; ++i)
				{
					for (int j = 0; j < numberofclients; ++j)
					{
						try
						{
							String queuename = L"queue.test." + String(j + 1);
							String testmessage = queuename + L".message." + String(i + 1);
							StringBuffer buf;
							for (int ll = 0; ll < 1; ++ll) buf->append(testmessage);
							TestServiceClient client = new TestServiceClientI(queuesession, queuename, buf->toString());
						}
						catch (const Exception& e)
						{
							cout << j + 1 << "." << i + 1 << " : " << e << endl;
							if (++exceptioncount > 10) break;							
						}
					}
					if (exceptioncount > 10) break;							
				}
				connection->stop();
			}
			else if (type_client == L"SERVER") 
			{
				try
				{
					for (int k = 0; k < 5; ++k)
					{
						cout << "run: " << k + 1 << endl;
						ServicesProvider servicesprovider;
						InitialContext initialcontext = InitialContext::newInstance();
						initialcontext->lookup(L"/servicesprovider/defaultservicesprovider", servicesprovider);

						servicesprovider->run();

						initialcontext->unloadBindings();
						initialcontext->loadBindings(L"");
					}
				}
				catch (const Exception& e)
				{
					cout << e << endl;
				}
			}
			else
			{
				String command = L"JMSTOOLSTESTservicesprovider.exe SERVER numberofservers busytimeout servicetimeout maxruntime bufferqueuesize | CLIENT numberofclients numberofrequests";
				cout << command << endl;
			}
		}

		jljlifetime->finalize();
	}
	catch (const Exception& e)
	{
		cout << e << endl;
	}

	return 0; 
}

//**********************************************************************

// eof
