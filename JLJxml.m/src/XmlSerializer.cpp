//XmlSerializer.cpp

//**********************************************************************

#ifndef jlj_xml_serializer_XmlSerializer_cpp
#define jlj_xml_serializer_XmlSerializer_cpp

//**********************************************************************

#include <jlj/xml/serialize/XmlSerializer.h>
#include <xercesc/util/XMLString.hpp>

//**********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(serialize)

//**********************************************************************

XmlSerializerI::~XmlSerializerI()
{}

//**********************************************************************

#define SETFEATURE(f, v) if (serializer->getDomConfig()->canSetParameter(f, v)) serializer->getDomConfig()->setParameter(f, v); else fprintf(stderr, "cannotset\n")

XmlSerializerI::XmlSerializerI(xercesc::DOMImplementation* implementation) throw (jlj::io::IOException)
: BaseMarkupSerializerI(implementation)
{
	output->setEncoding(X("UTF-8"));
	serializer->setNewLine(X(""));
#if defined(_WIN32)
	SETFEATURE(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, false);
	SETFEATURE(xercesc::XMLUni::fgDOMWRTWhitespaceInElementContent, false);
#endif
}

//**********************************************************************

void XmlSerializerI::setOutputFormat(const OutputFormat& outputformat)	 
{
	try
	{							 
		this->outputformat = outputformat;
		output->setEncoding(X(constcharptr(outputformat->getEncoding())));
		
		serializer->setNewLine(X(constcharptr(outputformat->getLineSeparator())));

#if defined(_WIN32)
		// crashes on AIX with xqilla
		SETFEATURE(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, outputformat->getPrettyPrint());
		SETFEATURE(xercesc::XMLUni::fgDOMWRTWhitespaceInElementContent, false);
#endif
	}
	catch (const xercesc::DOMException& e)
	{
		throw jlj::io::IOException(WITHDETAILS(String(e.getMessage(), 0)));
	}
}

//**********************************************************************

NAMESPACE_END(serialize)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//**********************************************************************

#endif