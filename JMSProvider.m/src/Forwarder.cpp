// Forwarder.cpp

//********************************************************************

#ifndef jmstools_jmsprovider_Forwarder_cpp
#define jmstools_jmsprovider_Forwarder_cpp

//********************************************************************

#include <Forwarder.h>
#include <jlj/net/InetSocketAddress.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/jms/TextMessage.h>
#include <jlj/jms/AnythingMessage.h>

using namespace jlj::util::logging;
using namespace jmstools::implementation::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

ForwarderI::~ForwarderI()
{}

//********************************************************************

ForwarderI::ForwarderI(jlj::nio::reactor::Connection connection, 
					   jlj::util::Anything anything,
					   bool jsonencoding)
					   : connection(connection)
					   , jsonencoding(jsonencoding)
					   , anything(anything)
{}

//********************************************************************

void ForwarderI::handle() throw (Exception)
{
	try
	{
		connection->send(anything, jsonencoding);
	}
	catch (const IOException& e)
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::WARNING, L"ForwarderI::handle() : " + e->toString());
	}
	catch (const Exception& e)
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::SEVERE, L"ForwarderI::handle() : " + e->toString());
	}
	catch (...)
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::SEVERE, L"ForwarderI::handle() : Unknown exception");
	}
}

//********************************************************************

void ForwarderI::run()
{
	handle();
}

//********************************************************************

NAMESPACE_END(jmstools)

#endif

// eof