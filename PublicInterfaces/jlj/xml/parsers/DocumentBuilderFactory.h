// DocumentBuilderFactory.h

//********************************************************************

#ifndef jlj_xml_parsers_DocumentBuilderFactory_h
#define jlj_xml_parsers_DocumentBuilderFactory_h

#include <jlj/xml/Exceptions.h>
#include <jlj/xml/parsers/DocumentBuilder.h>
#include <jlj/xml/serialize/DOMSerializer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(parsers)

//********************************************************************

class ExportedByJLJxml DocumentBuilder;

//********************************************************************

class ExportedByJLJxml DocumentBuilderFactoryI
    : public ObjectI
{
protected:
    ~DocumentBuilderFactoryI();	 
    friend class TypedReference<DocumentBuilderFactoryI>; 

public:
    DocumentBuilder newDocumentBuilder() throw (Exception);
	jlj::xml::serialize::DOMSerializer newDOMSerializer() throw (Exception);

private:
 	DocumentBuilderFactoryI();
	friend struct ExportedByJLJxml DocumentBuilderFactory;

private:
    xercesc::DOMImplementation* implementation;

};

//********************************************************************
//********************************************************************

struct ExportedByJLJxml DocumentBuilderFactory : StrongReference<DocumentBuilderFactoryI>
{
    StrongReferenceCommonDerivateImplementation(DocumentBuilderFactory);      
	static DocumentBuilderFactory newInstance();
};

//********************************************************************

NAMESPACE_END(parsers)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif
// eof


