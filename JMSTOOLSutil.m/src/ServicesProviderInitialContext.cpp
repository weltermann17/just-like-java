// ServicesProviderInitialContext.cpp

//********************************************************************

#ifndef ServicesProviderInitialContext_cpp
#define ServicesProviderInitialContext_cpp

//********************************************************************

#include <jmstools/util/services/ServicesProviderInitialContext.h>
#include <jlj/naming/InitialContext.h>
#include <jlj/lang/Thread.h>
#include <jlj/util/TreeSet.h>
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
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

ClassLoaderInformationDefinition(jmstools::util::services::ServicesProviderInitialContext, a95e096b58370f5a0b9ebeb210bdee5a)

//********************************************************************

ServicesProviderInitialContextI::~ServicesProviderInitialContextI()
{}

//********************************************************************

ServicesProviderInitialContextI::ServicesProviderInitialContextI()
{}

//********************************************************************

Object ServicesProviderInitialContextI::lookupClass(const String& name, 
													const String& clazzname) const throw (NamingException)
{
	try
	{
		if (L"jmstools::util::services::ServicesProvider" == clazzname)
		{
			bool createdbyfactory;
			String redirectedname;
			Object object = lookupObjectFactory(name, createdbyfactory, redirectedname);
			if (createdbyfactory)
			{
				ServicesProvider servicesprovider;
				object->downcast(servicesprovider);
				Anything configuration = new NullAnythingI();
				if (isBound(redirectedname + L"/configuration")) 
				{
					lookupURL(redirectedname + L"/configuration", configuration); 
				}
				servicesprovider->setConfiguration(configuration->toJSON());
				if (isBound(redirectedname + L"/services")) 
				{
					Anything services;
					lookupURL(redirectedname + L"/services", services);

					SortedSet<String> serviceids = new TreeSetI<String>();
					serviceids->addAll(services->keySet());
					Iterator<String> i = serviceids->iterator();
					while (i->hasNext())
					{
						String id = i->next();
						String servicename = services->get(id)->get(L"name"); 
						String lookupname = redirectedname + L"/services/" + id;
						String redirectedlookupname;
						Object o = lookupObjectFactory(lookupname, createdbyfactory, redirectedlookupname);
						if (createdbyfactory)
						{
							ServiceImplementor service;
							o->downcast(service);
							service->setName(servicename);
							Anything serviceconfiguration;
							if (isBound(redirectedlookupname + L"/configuration")) 
							{ 
								lookupURL(redirectedlookupname + L"/configuration", serviceconfiguration); 
								service->setConfiguration(serviceconfiguration->toJSON());
							}
							servicesprovider->registerService(servicename, service);
							InitialContext::newInstance()->unbind(lookupname);
						}
					}
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

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof