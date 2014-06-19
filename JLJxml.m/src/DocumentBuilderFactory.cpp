// DocumentBuilderFactory.cpp

//********************************************************************

#ifndef jlj_xml_parsers_DocumentBuilderFactory_cpp
#define jlj_xml_parsers_DocumentBuilderFactory_cpp

//********************************************************************

#include <jlj/xml/parsers/DocumentBuilderFactory.h>
#include <jlj/xml/serialize/XmlSerializer.h>
#include <jlj/util/Collections.h>
using namespace jlj::util;
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::lang::lifetimemanagement;
using namespace jlj::xml::serialize;
#include <xercesc/dom/DOM.hpp>
#include <xqilla/xqilla-dom3.hpp>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(parsers)

//********************************************************************

DocumentBuilderFactoryI::~DocumentBuilderFactoryI()
{
	try
	{
		implementation = 0;
		XQillaPlatformUtils::terminate();
	}
	catch (...)
	{
		fprintf(stderr, "DocumentBuilderFactoryI::~DocumentBuilderFactoryI() : Unknown exception.\n");
	}
}

//********************************************************************

DocumentBuilderFactoryI::DocumentBuilderFactoryI()
: implementation(0)
{
	try
	{
		XQillaPlatformUtils::initialize();
		implementation = xercesc::DOMImplementationRegistry::getDOMImplementation(X("XPath2 3.0 "));
		if (0 == implementation) throw Exception(WITHDETAILS(L"Invalid implementation string"));
	}
	catch (const Exception& e)
	{
		fprintf(stderr, "DocumentBuilderFactoryI::DocumentBuilderFactoryI() : %s\n", constcharptr(e->toString()));
	}
	catch (...)
	{
		fprintf(stderr, "DocumentBuilderFactoryI::DocumentBuilderFactoryI() : Unknown exception.\n");
	}
}

//********************************************************************

DocumentBuilder DocumentBuilderFactoryI::newDocumentBuilder() throw (Exception)
{
	return new DocumentBuilderI(implementation);
}

//********************************************************************

DOMSerializer DocumentBuilderFactoryI::newDOMSerializer() throw (Exception)
{
	return new XmlSerializerI(implementation);
}

//********************************************************************
//********************************************************************

DocumentBuilderFactory DocumentBuilderFactory::newInstance() 
{
	DocumentBuilderFactory documentbuilderfactory;

	try
	{
		SynchronizedMap<String, Object> namedsingletons;
		LifetimeManager::getInstance(L"jlj")->get(L"namedsingletons", namedsingletons);

		const String clazzname = L"jlj::xml::parsers::DocumentBuilderFactoryI";
		if (namedsingletons->containsKey(clazzname))
		{
			namedsingletons->get(clazzname)->downcast(documentbuilderfactory);
		}
		else
		{
			documentbuilderfactory = new jlj::xml::parsers::DocumentBuilderFactoryI();
			namedsingletons->put(clazzname, documentbuilderfactory);
		}
	} 
	catch (const Exception& e)
	{
		Logger::getLogger(Logger::GLOBAL_LOGGER_NAME)->log(Level::SEVERE, L"DocumentBuilderFactory::newInstance() : " + e->toString());
	}

	return documentbuilderfactory;
}

//********************************************************************

NAMESPACE_END(parsers)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof


