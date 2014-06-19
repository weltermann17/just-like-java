// XPathFactory.cpp

//**********************************************************************

#ifndef spi_XPathFactory_cpp
#define spi_XPathFactory_cpp

//**********************************************************************

#include <jlj/xml/xpath/spi/XPathFactory.h>
#include <jlj/util/Collections.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::util;
using namespace jlj::util::logging;
using namespace jlj::lang::lifetimemanagement;

//**********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)
NAMESPACE_BEGIN(spi)

//**********************************************************************

XPathFactoryI::~XPathFactoryI()
{}

//**********************************************************************

XPathFactoryI::XPathFactoryI()
{}

//**********************************************************************

jlj::xml::xpath::XPath XPathFactoryI::newXPath() 
{
	return new XPathI();
}

//**********************************************************************

NAMESPACE_END(spi)
NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)


//**********************************************************************
//**********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)

//**********************************************************************

XPathFactoryI::~XPathFactoryI()	 
{}

//**********************************************************************

XPathFactoryI::XPathFactoryI()
{}

//**********************************************************************

XPathFactory XPathFactory::newInstance() 
{
	XPathFactory xpathfactory;

	try
	{
		SynchronizedMap<String, Object> namedsingletons;
		LifetimeManager::getInstance(L"jlj")->get(L"namedsingletons", namedsingletons);

		const String clazzname = L"jlj::xml::xpath::spi::XPathFactoryI";

		if (namedsingletons->containsKey(clazzname))
		{
			namedsingletons->get(clazzname)->downcast(xpathfactory);
		}
		else
		{
			xpathfactory = new jlj::xml::xpath::spi::XPathFactoryI();
			namedsingletons->put(clazzname, xpathfactory);
		}
	} 
	catch (const Exception& e)
	{
		Logger::getLogger(Logger::GLOBAL_LOGGER_NAME)->log(Level::SEVERE, L"XPathFactory::newInstance() : " + e->toString());
	}

	return xpathfactory;
}

//**********************************************************************

NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//**********************************************************************

#endif

// eof


