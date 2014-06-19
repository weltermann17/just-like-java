// main.cpp
//*********************************************************************

#include <JMSToolstests.h>

//**********************************************************************

#include <cppunit/ui/text/TestRunner.h>

#include <jlj/lang/Thread.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;

#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/lifetime/JLJLifetimeImplementation.h>
using namespace jlj::lifetime;

#include <jlj/util/StringAnything.h>
using namespace jlj::util;
using namespace jlj::util::logging;

#include <JMSTest.h>
using namespace jmstools;

#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

//**********************************************************************

int main(int argc, char** argv) 
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

	try
	{
		String command = L"Command : JMSToolstests.exe REQ [ STRING MIN_SIZE MAX_SIZE | STREAM FILE_PATH ] MAINLOOP SUBMITLOOP INNERLOOP \n";
		command = command + L"Command : JMSToolstests.exe REP SUBMITLOOP \n";

		if (1 < argc)
		{	
			String type_client = String(argv[1], strlen(argv[1]));

			if (type_client== L"REQ") 
			{
				String type_stream = String(argv[2], strlen(argv[2]));
				if (type_stream == L"STRING" && 8 == argc)
				{

					int min_blocksize = (new StringAnythingI(String(argv[3], strlen(argv[3]))))->operator int();
					int max_blocksize = (new StringAnythingI(String(argv[4], strlen(argv[4]))))->operator int();
					if (max_blocksize <= min_blocksize) max_blocksize = min_blocksize + 1;

					int outerloop = (new StringAnythingI(String(argv[5], strlen(argv[5]))))->operator int();
					int submitloop = (new StringAnythingI(String(argv[6], strlen(argv[6]))))->operator int();
					int innerloop  = (new StringAnythingI(String(argv[7], strlen(argv[7]))))->operator int();

					JMSTest jmstest = new JMSTestI(
						type_stream,
						L"",
						outerloop,
						submitloop,
						innerloop,
						min_blocksize,
						max_blocksize);
					jmstest->RequestConnection();
				}
#if 0
				else if (type_stream == L"STREAM" && 6 == argc)
				{

					String path_file = String(argv[3], strlen(argv[3]));
					cout << "path_file :" << path_file << endl;

					int outterloop = (new StringAnythingI(String(argv[4],strlen(argv[4]))))->operator int();
					int innerloop  = (new StringAnythingI(String(argv[5],strlen(argv[5]))))->operator int();

					JMSTest jmstest=new JMSTestI(type_stream,path_file,outterloop,0,0,innerloop);
					jmstest->RequestConnection();
				}
#endif
			}
			else if (type_client == L"REP" && argc == 3) 
			{
				int submitloop = (new StringAnythingI(String(argv[2], strlen(argv[2]))))->operator int();

				JMSTest jmstest = new JMSTestI(
					L"",
					L"",
					0,
					submitloop,
					0,
					0,
					0);

				jmstest->ReplyConnection();
			}
			else if (type_client == L"-h")
			{
				cout << command << endl;
			}
			else
			{
				cout << "Error Syntax Parameter " << endl;
				cout << command << endl;
				return 0;
			}
		}
	}
	catch (const Exception& e)
	{
		cout << "main() : " << e->toString() << endl;
	}

	jljlifetime->finalize();

	return 0; 
}

//**********************************************************************

// eof
