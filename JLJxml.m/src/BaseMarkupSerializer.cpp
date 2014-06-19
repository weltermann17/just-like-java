// BaseMarkupSerializer.cpp

//***************************************************************************

#ifndef jlj_xml_serializer_BaseMarkupSerializer_cpp
#define jlj_xml_serializer_BaseMarkupSerializer_cpp

//***************************************************************************

#include <jlj/xml/serialize/BaseMarkupSerializer.h>
#include <jlj/xml/parsers/DocumentBuilderFactory.h>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
using namespace jlj::xml::parsers;

//***************************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(serialize)

//***************************************************************************

BaseMarkupSerializerI::~BaseMarkupSerializerI()
{
	if (0 != target) target->flush();
	delete target;
}

//***************************************************************************

BaseMarkupSerializerI::BaseMarkupSerializerI(xercesc::DOMImplementation* implementation) throw (jlj::io::IOException)
: serializer(0 == implementation ? 0 : implementation->createLSSerializer())
, output(0 == implementation ? 0 : implementation->createLSOutput())
, target(0)
{
	if (0 == implementation) throw jlj::io::IOException(WITHDETAILS(L"xercesc::DOMImplementation not initialized"));
}

//***************************************************************************

void BaseMarkupSerializerI::serialize(Node node) throw (jlj::io::IOException)
{  
	if (0 == target) throw jlj::io::IOException(WITHDETAILS(L"setOutputCharStream() must be called before"));

	try
	{	
		serializer->write(node, output);
	}
	catch (const xercesc::XMLException& e)
	{
		throw jlj::io::IOException(WITHDETAILS(String(e.getMessage(), 0)));
	}
	catch (const xercesc::DOMException& e)
	{
		throw jlj::io::IOException(WITHDETAILS(String(e.getMessage(), 0)));
	}
	catch (const Exception& e)
	{
		throw jlj::io::IOException(WITHDETAILS(e->toString()));
	}
	catch (...)
	{
		throw jlj::io::IOException(WITHDETAILS(L"BaseMarkupSerializer::serialize() : Unknown exception."));
	}
}

//***************************************************************************

void BaseMarkupSerializerI::serialize(const jlj::util::List<Node>& list) throw (jlj::io::IOException)
{  
	jlj::util::Iterator<Node> i = list->iterator();
	while (i->hasNext())
	{
		serialize(i->next());
	}
}

//***************************************************************************

void BaseMarkupSerializerI::setOutputCharStream(const String& filename) throw (jlj::io::IOException)
{
	try
	{
		target = new xercesc::LocalFileFormatTarget(X(constcharptr(filename)));
		output->setByteStream(target);
	}
	catch (const xercesc::DOMException& e)
	{
		throw jlj::io::IOException(WITHDETAILS(String(e.getMessage(), 0)));
	}
	catch (...)
	{
		throw jlj::io::IOException(WITHDETAILS(L"BaseMarkupSerializer::setOutputCharStream() : Unknown exception."));
	}
}

//***************************************************************************
//***************************************************************************

DOMSerializerI::~DOMSerializerI()
{}

//***************************************************************************

DOMSerializerI::DOMSerializerI()
{}

//***************************************************************************

NAMESPACE_END(serialize)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//***************************************************************************

#endif

// eof


