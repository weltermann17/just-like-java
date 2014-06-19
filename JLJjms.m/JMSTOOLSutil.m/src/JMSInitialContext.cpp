// JMSInitialContext.cpp

//********************************************************************

#ifndef JMSInitialContext_cpp
#define JMSInitialContext_cpp

//********************************************************************

#include <jmstools/naming/JMSInitialContext.h>
#include <jlj/naming/InitialContext.h>
#include <jlj/lang/Thread.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/jms/Message.h>
#include <jlj/jms/QueueConnectionFactory.h>
#include <jlj/jms/ConnectionFactory.h>
#include <jlj/io/FileInputStream.h>
using namespace jlj::io;
using namespace jlj::naming;
using namespace jlj::util;
using namespace jlj::util::logging;
using namespace jlj::jms;

#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(naming)

//********************************************************************

ClassLoaderInformationDefinition(jmstools::naming::JMSInitialContext, d67e0837626fe9efa63ad5cec592cbe4)

//********************************************************************

JMSInitialContextI::~JMSInitialContextI()
{}

//********************************************************************

JMSInitialContextI::JMSInitialContextI()
{}

//********************************************************************

Object JMSInitialContextI::lookupClass(const String& name, const String& clazzname) const throw (NamingException)
{
	try
	{
		if (L"jlj::jms::QueueConnectionFactory" == clazzname || L"jlj::jms::ConnectionFactory" == clazzname)
		{
			bool createdbyfactory;
			String redirectedname;
			Object object = lookupObjectFactory(name, createdbyfactory, redirectedname);
			if (createdbyfactory)
			{
				ConnectionFactory connectionfactory;
				object->downcast(connectionfactory);
				Anything configuration = new NullAnythingI();
				InitialContext initialcontext = InitialContext::newInstance();
				if (verboseOutput()) cout << "JNDI context : Testing for JMS_CONNECTION_STRING."<< endl;
				if (initialcontext->isBound(L"/env/jms_connection_string"))
				{
					Anything connectionstring;
					initialcontext->lookup(L"/env/jms_connection_string", connectionstring);
					configuration = Anything::decodeJSON(connectionstring, true);
				}
				else if (initialcontext->isBound(L"/env/jms_provider_host"))
				{
					if (verboseOutput()) cout << "JNDI context : JMS_CONNECTION_STRING not set"<< endl;
					Anything conf = new MapAnythingI();
					Anything host;
					initialcontext->lookup(L"/env/jms_provider_host", host);
					conf->put(L"providerhost", host);
					if (initialcontext->isBound(L"/env/jms_provider_port"))
					{
						Anything port;
						initialcontext->lookup(L"/env/jms_provider_port", port);
						conf->put(L"providerport", port);
					} 
					else
					{
						if (verboseOutput()) cout << "JNDI context : JMS_PROVIDER_PORT not set"<< endl;
					}
					configuration = conf;
				}
				else if (initialcontext->isBound(L"/env/jms_provider_port"))
				{
					if (verboseOutput()) cout << "JNDI context : JMS_PROVIDER_HOST not set"<< endl;
					Anything conf = new MapAnythingI();
					Anything port;
					initialcontext->lookup(L"/env/jms_provider_port", port);
					conf->put(L"providerport", port);
					configuration = conf;
				}
				else if (isBound(redirectedname + L"/configuration")) 
				{
					if (verboseOutput()) cout << "JNDI context : testing for" << redirectedname << "/configuration" << endl;
					lookupURL(redirectedname + L"/configuration", configuration); 
				}
				if (Anything::ANY_STRING == configuration->type())
				{
					connectionfactory->setConfiguration(configuration->toString());
				}
				else
				{
					connectionfactory->setConfiguration(configuration->toJSON());
				}
			}
			return object;
		}
		else
		{
			return AbstractContextI::lookupClass(name, clazzname);
		}
	}
	catch (const NamingException&)
	{
		throw;
	}
	catch (const Exception& e)
	{
		throw NamingException(e->toString());
	}
}

//********************************************************************

void JMSInitialContextI::loadBindings(const String& configuration) throw (Exception)
{
	// extras for environment variables and backward compatability

	String goodconfig;
	if (isBound(L"/env/jms_context_file"))
	{
		Anything configfilename;
		lookup(L"/env/jms_context_file", configfilename);
		InputStream configfile = new FileInputStreamI(configfilename);
		UTF8String config = "null";
		configfile->read(config, 0);
		goodconfig = config->toString();
	}
	else
	{
		goodconfig = configuration;
	}

	AbstractContextI::loadBindings(goodconfig);

	// environment has highest precedence and comes, therefore, last

	InitialContext initialcontext = InitialContext::newInstance();
	initialcontext->copyBinding(L"/env/jms_connectionfactory", L"/jms/defaultconnectionfactory");
}

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof