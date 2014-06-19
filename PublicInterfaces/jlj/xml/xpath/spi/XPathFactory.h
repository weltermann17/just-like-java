// XPathFactory.h

//********************************************************************

#ifndef spi_XPathFactory_h
#define spi_XPathFactory_h

//********************************************************************

#include <jlj/xml/xpath/spi/XPath.h>
#include <jlj/xml/xpath/XPathFactory.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)
NAMESPACE_BEGIN(spi)

//********************************************************************

class ExportedByJLJxml XPathFactoryI
    : public jlj::xml::xpath::XPathFactoryI
{

protected:
    ~XPathFactoryI();
    friend class TypedReference<XPathFactoryI>; 

public:
    XPathFactoryI();	
    jlj::xml::xpath::XPath newXPath(); 

};

//********************************************************************

StrongReferenceCommonDeclaration(XPathFactory, ExportedByJLJxml);

//********************************************************************

NAMESPACE_END(spi)
NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif
// eof


