// AbstractServiceImplementor.cpp

//********************************************************************

#ifndef AbstractServiceImplementor_cpp
#define AbstractServiceImplementor_cpp

//********************************************************************

#include <jmstools/util/services/AbstractServiceImplementor.h>
#include <jlj/util/MapAnything.h>
using namespace jlj::util;
#include <jlj/jms/Message.h>
using namespace jlj::jms;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

AbstractServiceImplementorI::~AbstractServiceImplementorI()
{
	if (!!logger) 
	{
		logger->log(Level::FINEST, L"~AbstractServiceImplementorI() : " + getName());
		Logger::removeLogger(logger->getName());
		logger = Logger::getLogger(Logger::GLOBAL_LOGGER_NAME);
	}
}

//********************************************************************

AbstractServiceImplementorI::AbstractServiceImplementorI() 
: outparameters(new MapAnythingI())
, inparameters(new MapAnythingI())
{
	logger = Logger::getLogger(Logger::GLOBAL_LOGGER_NAME);
}

//********************************************************************

String AbstractServiceImplementorI::doGet(const String&) throw (Exception)
{
	throw JMSException(WITHDETAILS(L"ServiceImplementor : doGet not implemented"));
}

//********************************************************************

String AbstractServiceImplementorI::doHead(const String&) throw (Exception)
{
	throw JMSException(WITHDETAILS(L"ServiceImplementor : doGet not implemented"));
}

//********************************************************************

String AbstractServiceImplementorI::doOptions(const String&) throw (Exception)
{
	throw JMSException(WITHDETAILS(L"ServiceImplementor : doOptions not implemented"));
}

//********************************************************************

String AbstractServiceImplementorI::doPost(const String&) throw (Exception)
{
	throw JMSException(WITHDETAILS(L"ServiceImplementor : doPost not implemented"));
}

//********************************************************************

String AbstractServiceImplementorI::doPut(const String&) throw (Exception)
{
	throw JMSException(WITHDETAILS(L"ServiceImplementor : doPut not implemented"));
}

//********************************************************************

String AbstractServiceImplementorI::doDelete(const String&) throw (Exception)
{
	throw JMSException(WITHDETAILS(L"ServiceImplementor : doDelete not implemented"));
}

//********************************************************************

String AbstractServiceImplementorI::doService(const String& in) throw (Exception)
{
	if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, getName() + L" : " + in + getInParameters()->toString());

	String httpmethod = getInParameters()->get(L"com.ibm.de.ebs.plm.scala.rest.http.method")->toString()->toLowerCase();

	if (L"get" == httpmethod) 
	{
		return doGet(in);
	}
	else if (L"head" == httpmethod) 
	{
		return doHead(in);
	}
	else if (L"options" == httpmethod) 
	{
		return doOptions(in);
	}
	else if (L"post" == httpmethod) 
	{
		return doPost(in);
	}
	else if (L"put" == httpmethod) 
	{
		return doPut(in);
	}
	else if (L"delete" == httpmethod) 
	{
		return doDelete(in);
	}
	else 
	{
		logger->log(Level::SEVERE, getName() + L" : HTTP method not implemented: " + httpmethod->toUpperCase());
		throw JMSException(WITHDETAILS(getName() + L" : HTTP method not implemented: " + httpmethod->toUpperCase()));
	}
}

//********************************************************************

Anything AbstractServiceImplementorI::getInParameters() const  
{
	return inparameters;
}

//********************************************************************

Anything AbstractServiceImplementorI::getOutParameters() const  
{
	return outparameters;
}

//********************************************************************

void AbstractServiceImplementorI::setInParameters(const Anything& inparameters)  
{
	this->inparameters = inparameters;
}

//********************************************************************

String AbstractServiceImplementorI::getName() const 
{
	return name;
}

//********************************************************************

void AbstractServiceImplementorI::setName(const String& name)
{
	this->name = name;
	logger = Logger::getLogger(name);
}

//********************************************************************

void AbstractServiceImplementorI::setConfiguration(const String&) throw (Exception)
{}

//********************************************************************

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof