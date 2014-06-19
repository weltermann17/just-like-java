// DocumentBuilder.cpp

//********************************************************************

#ifndef jlj_xml_parsers_DocumentBuilder_cpp
#define jlj_xml_parsers_DocumentBuilder_cpp

//********************************************************************

#include <jlj/xml/parsers/DocumentBuilder.h>
#include <jlj/xml/parsers/DocumentBuilderFactory.h>
#include <jlj/io/InMemoryOutputStream.h>
#include <jlj/io/UTF8StringInputStream.h>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(parsers)

//********************************************************************

const int DocumentBuilderI::DEFAULTBUFFERSIZE = 8*1024;

//********************************************************************

DocumentBuilderI::~DocumentBuilderI()
{
	if (0 != parser) parser->release();
}

//********************************************************************

DocumentBuilderI::DocumentBuilderI(xercesc::DOMImplementation* implementation) throw (Exception)
{
#define SETFEATURE(f) if (parser->getDomConfig()->canSetParameter(f, true)) parser->getDomConfig()->setParameter(f, true)

	if (0 == implementation) throw ParserConfigurationException(WITHDETAILS(L"xercesc::DOMImplementation not initialized."));
	
	try
	{
		parser = implementation->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
		SETFEATURE(xercesc::XMLUni::fgDOMValidate);
		SETFEATURE(xercesc::XMLUni::fgDOMNamespaces);
		SETFEATURE(xercesc::XMLUni::fgDOMDatatypeNormalization);
		SETFEATURE(xercesc::XMLUni::fgXercesDOMHasPSVIInfo);
		SETFEATURE(xercesc::XMLUni::fgXercesSchema);
		SETFEATURE(xercesc::XMLUni::fgDOMValidateIfSchema);
	}
	catch (const xercesc::DOMException& e)
	{
		throw Exception(WITHDETAILS(String(e.getMessage(), 0)));
	}
	catch (...)
	{
		throw Exception(WITHDETAILS(L"DocumentBuilderI::DocumentBuilderI() : Unknown exception."));
	}

#undef SETFEATURE
}

//********************************************************************

Document DocumentBuilderI::parse(const String& uri) throw (jlj::io::IOException, jlj::xml::SAXException)
{ 
	try
	{
		parser->resetDocumentPool();
		Document document = parser->parseURI(X(constcharptr(uri)));
		if (0 == document) throw jlj::io::IOException(WITHDETAILS(L"Invalid uri : " + uri));
		return document;		
	}
	catch (const jlj::io::IOException&)
	{
		throw;
	}
	catch (const jlj::lang::Exception& e)
	{
		throw jlj::io::IOException(WITHDETAILS(e->toString()));
	}
	catch (const xercesc::XMLException& e)
	{
		throw SAXException(WITHDETAILS(String(e.getMessage(), 0)));
	}
	catch (const xercesc::DOMException& e)
	{
		throw SAXException(WITHDETAILS(String(e.getMessage(), 0)));
	}
	catch (...)
	{
		throw jlj::io::IOException(WITHDETAILS(L"DocumentBuilderI::parse(URI) : Unknown exception."));
	}
}

//********************************************************************

Document DocumentBuilderI::parse(jlj::io::InputStream is) throw (jlj::io::IOException, SAXException)
{ 
#define CLEANUP_BUFFERS delete domIS; delete memBuf; delete [] buffer; delete document

	char* buffer = 0;
	Document document = 0;
	xercesc::MemBufInputSource* memBuf = 0;

	xercesc::DOMLSInput* domIS = 0;
	try
	{
		int buffersize = DEFAULTBUFFERSIZE < is->available() ? is->available() + 1 : DEFAULTBUFFERSIZE;

		buffer = new char[buffersize];
		int l = buffersize;
		int n = 0;
		int total = 0;

		while (0 < (n = is->read(buffer + total, 0, l)))
		{
			l -= n;
			total += n;

			if (0 == l)
			{
				char* oldbuffer = buffer;
				buffer = new char[buffersize * 2];
				memcpy(buffer, oldbuffer, total);
				delete [] oldbuffer;
				buffersize *= 2;
				l = buffersize - total;
			}
		}

		if (0 < total)		 
		{
			memBuf = new xercesc::MemBufInputSource(                                                       
				(const XMLByte*) buffer, 
				total, 
				"ID", 
				false);
			memBuf->setCopyBufToStream(false);
			domIS = new xercesc::Wrapper4InputSource(memBuf, false); 
			parser->resetDocumentPool();
			document = parser->parse(domIS);
			delete domIS;			
			delete memBuf;		
		}	

		delete [] buffer;					
		return document;
	}																					    
	catch (const jlj::io::IOException&)
	{
		CLEANUP_BUFFERS;
		throw;
	}
	catch (const jlj::lang::Exception& e)
	{
		CLEANUP_BUFFERS;
		throw jlj::io::IOException(WITHDETAILS(e->toString()));
	}
	catch (const xercesc::XMLException& e)
	{
		CLEANUP_BUFFERS;
		throw SAXException(WITHDETAILS(String(e.getMessage(), 0)));
	}
	catch (const xercesc::DOMException & e)
	{
		CLEANUP_BUFFERS;
		throw SAXException(WITHDETAILS(String(e.getMessage(), 0)));
	}
	catch (...)
	{
		CLEANUP_BUFFERS;
		throw jlj::io::IOException(WITHDETAILS(L"DocumentBuilderI::parse(InputStream) : Unknown exception."));
	}
#undef CLEANUP_BUFFERS 
}

//********************************************************************

Document DocumentBuilderI::parseUTF8String(UTF8String& xmlstring) throw (jlj::io::IOException, SAXException)
{ 
	return parse(new jlj::io::UTF8StringInputStreamI(xmlstring));
}

//********************************************************************

NAMESPACE_END(parsers)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif