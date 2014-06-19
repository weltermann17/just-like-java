//DocumentBuilder.h

//********************************************************************

#ifndef jlj_xml_parsers_DocumentBuilder_h
#define jlj_xml_parsers_DocumentBuilder_h

//********************************************************************

#include <jlj/xml/Typedef.h>
#include <jlj/xml/Exceptions.h>
#include <jlj/io/Exceptions.h>
#include <jlj/io/InputStream.h>

#include <xercesc/dom/DOM.hpp>


using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(parsers)

//********************************************************************

class ExportedByJLJxml DocumentBuilderI
    : public ObjectI
{
protected:
    ~DocumentBuilderI(); 
    friend class TypedReference<DocumentBuilderI>;

public:
    Document parse(const String& uri) throw (jlj::io::IOException, jlj::xml::SAXException);
    Document parse(jlj::io::InputStream) throw (jlj::io::IOException, jlj::xml::SAXException);

	// parseUTF8String will consume the xmlstring by swapping its buffer, after this call it will be empty
    Document parseUTF8String(UTF8String& xmlstring) throw (jlj::io::IOException, jlj::xml::SAXException);

private:	
    DocumentBuilderI(xercesc::DOMImplementation*) throw (Exception);
    friend class ExportedByJLJxml DocumentBuilderFactoryI;

	xercesc::DOMLSParser* parser;
    static const int DEFAULTBUFFERSIZE;

};

//********************************************************************

StrongReferenceCommonDeclaration(DocumentBuilder, ExportedByJLJxml);

//********************************************************************

NAMESPACE_END(parsres)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif
