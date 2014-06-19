//ProviderManager.cpp

//********************************************************************

#ifndef jmstools_ProviderManager_cpp
#define jmstools_ProviderManager_cpp

//********************************************************************

#include <ProviderManager.h>
#include <jmstools/implementation/reactor/Queue.h>
#include <jlj/jms/QueueConnectionFactory.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/jms/Exceptions.h>
#include <jmstools/implementation/reactor/AnythingMessage.h>
#include <jmstools/implementation/reactor/TextMessage.h>
#include <jlj/util/StopWatch.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>


using namespace std;
using namespace jlj::util;
using namespace jlj::jms;
//*****************************************************

NAMESPACE_BEGIN(jmstools)

//*****************************************************

ProviderManagerI::~ProviderManagerI()
{
	connection->close();
}

//***********************************************************

ProviderManagerI::ProviderManagerI(QueueConnectionFactory queueconnectionfactory,
								   Logger logger)
								   : queueconnectionfactory(queueconnectionfactory)
								   , logger(logger)
{}

//***********************************************************************

void ProviderManagerI::createControlConnection() throw (JMSException)
{
	queue = new jmstools::implementation::reactor::QueueI(L"CONTROL", jmstools::implementation::reactor::Message::DEST_CONTROL);
	connection = queueconnectionfactory->createQueueConnection();
	connection->start();
	session = connection->createQueueSession();
}

//***********************************************************************

QueueSender ProviderManagerI::createQueueSender() throw (JMSException)
{
	createControlConnection();
	return session->createSender(queue);
}

//***********************************************************************

AnythingMessage ProviderManagerI::createMessage(Anything message) throw (JMSException)
{

	AnythingMessage msg = session->createAnythingMessage(message);
	msg->setJMSDestination(queue);
	return msg;
}

//****************************************************************************
int ProviderManagerI::shutdown(int timeout) throw (JMSException)
{
	int errorcode = jmstools::implementation::reactor::Message::FAILURE;

	logger->log(Level::INFO, L"ProviderManagerI::shutdown()");
	createControlConnection();
	jmstools::implementation::reactor::AnythingMessage request = new jmstools::implementation::reactor::AnythingMessageI(); 
	request->setCommand(jmstools::implementation::reactor::Message::COMMAND_SHUTDOWN);
	QueueRequestor requestor = session->createQueueRequestor(queue);
	AnythingMessage reply;
	requestor->request(request, timeout)->downcast(reply);

	try
	{
		if (!!reply)
		{
			errorcode = (new StringAnythingI(reply->getAnything()->get(L"ERRORCODE")->toString()))->operator int();
			cout << "Error Code Receive : " << reply->getAnything()->get(L"ERRORCODE")->toString() << endl;
		}
	}
	catch(IndexOutOfBoundsException& e)
	{
		throw jlj::jms::JMSException(WITHDETAILS(e->toString()));
	}
	return(errorcode);
}
//****************************************************************************
int ProviderManagerI::monitor(int timeout, bool nagios) throw (JMSException)
{
	int errorcode = jmstools::implementation::reactor::Message::FAILURE;
	StopWatch stopwatch = new StopWatchI();

	stopwatch->start();
	createControlConnection();
	jmstools::implementation::reactor::AnythingMessage request = new jmstools::implementation::reactor::AnythingMessageI();
	int command;
	if (nagios) command = jmstools::implementation::reactor::Message::COMMAND_MONITOR_FORNAGIOS;
	else command = jmstools::implementation::reactor::Message::COMMAND_MONITOR;
	request->setCommand(command);
	QueueRequestor requestor = session->createQueueRequestor(queue);
	AnythingMessage reply;
	requestor->request(request, timeout)->downcast(reply);
	try
	{
		if (!!reply)
		{
			stopwatch->stop();
			String message = reply->getAnything()->get(L"MESSAGE")->toString();
			String rta;
			if (nagios) 
			{
				rta = L";;;; rta=" + String(stopwatch->getSeconds()) + L"s;;;;";
			}
			else
			{
				rta = L", rta " + String(stopwatch->getSeconds())+ L"s";
			}
			cout << message << rta << endl;
			errorcode = (new StringAnythingI(reply->getAnything()->get(L"ERRORCODE")->toString()))->operator int();
		}
	}
	catch(IndexOutOfBoundsException& e)
	{
		throw jlj::jms::JMSException(WITHDETAILS(e->toString()));
	}
	return(errorcode);
}
//**********************************************************************

void ProviderManagerI::setGlobalLogLevel(String level) throw (JMSException)
{

	sendLogLevel(jmstools::implementation::reactor::Message::COMMAND_SETGLOBALLOGLEVEL, level);
}

//**********************************************************************

void ProviderManagerI::setReactorLogLevel(String level) throw (JMSException)
{
	sendLogLevel(jmstools::implementation::reactor::Message::COMMAND_SETREACTORLOGLEVEL, level);
}

//**********************************************************************

void ProviderManagerI::setLogLevel(String level) throw (JMSException)
{
	sendLogLevel(jmstools::implementation::reactor::Message::COMMAND_SETLOGLEVEL, level);
}

//**********************************************************************

void ProviderManagerI::setConsoleHandlerLogLevel(String path) throw (JMSException)
{
	sendLogLevel(jmstools::implementation::reactor::Message::COMMAND_SETCONSOLEHANDLERLOGLEVEL, path);
}
//**********************************************************************

void ProviderManagerI::sendLogLevel(int command, String level) throw (JMSException)
{
	logger->log(Level::INFO, L"ProviderManagerI::sendLogLevel()");
	createControlConnection();
	jmstools::implementation::reactor::AnythingMessage message = new jmstools::implementation::reactor::AnythingMessageI(); 
	message->setCommand(command);
	message->setCommandParameter(0, new StringAnythingI(level));
	createQueueSender()->send(message);
}

//**********************************************************************

bool ProviderManagerI::isLogLevelAllowed(String level) const
{
	level = level->toUpperCase();
	return	(level == L"OFF" ||
		level == L"SEVERE" ||
		level == L"WARNING" ||
		level == L"INFO" ||
		level == L"FINE" ||
		level == L"FINER" ||
		level == L"FINEST" ||
		level == L"CONFIG" ||
		level == L"ALL" ) ? true : false;
}

//**********************************************************************

bool ProviderManagerI::isLoggingFile(const String& file) const
{
	StringBuffer logfile(file);
	cout << "isLoggingFile" << endl;
	logfile = logfile->substring(String(L".txt")->length() , file->length());
	cout << logfile << endl;
	return	(logfile == L".txt" || logfile == L".log") ? true : false;
}

//**********************************************************************

int ProviderManagerI::getTimeOut(const String& parameter) const
{
	int timeout;
	if (0 == parameter->length() || isNagiosOn(parameter)) 
	{
		timeout = 1000;
	}
	else
	{
		timeout = (new StringAnythingI(parameter))->operator int();
	}
	return timeout;
}

//**********************************************************************

bool ProviderManagerI::isNagiosOn(const String& parameter) const
{
	return ( parameter == L"-nagios" || parameter == L"-n") ? true : false;
}

//**********************************************************************

void ProviderManagerI::usage()
{
	std::cout << "\nUsage: JMSProviderManager -[COMMAND] [OPTION] -[OUTPUT]\n"\
		"\nCOMMANDS:\n"\
		"****************************************************** \n"\
		"-help or -h			\tUsage\n"\
		"-shutdown or -s [time in secs]	 \tAnswer Shutdown Timeout in X msecs (default 1000 ms)\n"\
		"-monitor or -m [time in secs] -[OUTPUT] \tAnswer Monitor Timeout in X msecs (default 1000 ms) with output for nagios (default=false)\n"\
		"-consolehandler or -ch [filepath] \tSet the outputfile for logging (default file logging.log)\n"\
		"-loglevel or -l [loglevel]	\tSet the global and\n"\
		"				\tthe local Logger Level to level (default SEVERE)\n"\
		"-globalloglevel or -gl [loglevel]\tSet the Global Logger Level to level (default SEVERE)\n"\
		"-reactorloglevel or -rl [loglevel]\tSet the Reactor Logger Level to level (default SEVERE)\n"\
		"loglevels : (OFF | SEVERE | WARNING | INFO | FINE | FINER | FINEST | CONFIG| ALL)\n"\
		"****************************************************** \n"\
		"\nOUTPUT:\n"\
		"****************************************************** \n"\
		"-nagios or -n			\tOutput for nagios\n"\
		"****************************************************** \n"\
		<< std::endl;	
}

//*******************************************************************

void ProviderManagerI::start(const String& command, const String& parameter, bool nagios) throw (JMSException)
{


	if (command == L"-help" || command == L"-h")
	{
		usage();
	}
	else if (command == L"-shutdown" || command == L"-s")
	{
		int timeout = getTimeOut(parameter);
		if (0 == timeout){usage();}
		else {if (jmstools::implementation::reactor::Message::FAILURE == shutdown(timeout)) throw JMSException(L"Shutdown failed");}
	}
	else if ( command == L"-loglevel" || command == L"-l")
	{
		if (!isLogLevelAllowed(parameter)) {usage();}
		else setLogLevel(parameter->toUpperCase());
	}
	else if ( command == L"-globalloglevel" || command == L"-gl")
	{
		if (!isLogLevelAllowed(parameter))  {usage();}
		else setGlobalLogLevel(parameter->toUpperCase());
	}
	else if ( command == L"-reactorloglevel" || command == L"-rl")
	{
		if (!isLogLevelAllowed(parameter))  {usage();}
		else setReactorLogLevel(parameter->toUpperCase());
	}
	else if (command == L"-monitor" || command == L"-m")
	{

		int timeout = getTimeOut(parameter);
		if (0 == timeout)   {usage();}
		else {if (jmstools::implementation::reactor::Message::FAILURE == monitor(timeout, nagios)) throw JMSException(L"Monitoring failed");}
	}
	else if (command == L"-ch" || command == L"-consolehandler")
	{
		if (parameter->length() == 0) 
		{
			setConsoleHandlerLogLevel(L"loggingfile.log");
		}
		else if (isLoggingFile(parameter)) 
		{
			setConsoleHandlerLogLevel(parameter);
		}
		else 
		{
			usage();
		}	
	}
	else
	{
		logger->log(Level::SEVERE, L"The command is not right. Please follow the usage of this programm");
		usage();
	}
}

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************
#endif

// eof