// XPathFactory.h

//********************************************************************

#ifndef jlj_xml_xpath_XPathFactory_h
#define jlj_xml_xpath_XPathFactory_h

//********************************************************************

#include <jlj/xml/xpath/XPath.h>
#include <jlj/util/AbstractNamedSingleton.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)

//********************************************************************
class ExportedByJLJxml XPathFactoryI
    : public ObjectI

{
public:
    virtual ~XPathFactoryI() = 0;
protected:
    XPathFactoryI();

public:
    virtual XPath newXPath() = 0;

};

//********************************************************************
//********************************************************************

struct ExportedByJLJxml XPathFactory : StrongReference<XPathFactoryI>
{
    StrongReferenceCommonDerivateImplementation(XPathFactory);
    static XPathFactory newInstance();
};

//********************************************************************

NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)



//********************************************************************

#endif

// eof


