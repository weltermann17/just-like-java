//XPathI.h

//********************************************************************

#ifndef spi_XPathI_h
#define spi_XPathI_h

//********************************************************************

#include <jlj/xml/xpath/XPath.h>
#if 0
#include <jlj3rd/xalanc/XalanDOM/XalanDocument.hpp>
#include <jlj3rd/xalanc/XPath/NodeRefList.hpp>
#include <jlj3rd/xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <jlj3rd/xalanc/XPath/XPathEvaluator.hpp>
#include <jlj3rd/xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#endif

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)
NAMESPACE_BEGIN(spi)

//********************************************************************

class ExportedByJLJxml XPathI
	: public jlj::xml::xpath::XPathI
{
protected:
	~XPathI(); 
	friend class TypedReference<XPathI>; 

public:
	XPathI();

public:
	String evaluateToString(const String& xpath, Document document) throw (Exception);
	jlj::util::List<String> evaluateToStringList(const String& xpath, Document document) throw (Exception);
	Node evaluateToNode(const String& xpath, Document document) throw (Exception);
	jlj::util::List<Node> evaluateToNodeList(const String& xpath, Document document) throw (Exception);
	jlj::util::Map<String, String> evaluateToAttributesMap(const String& expression, Document) throw (Exception);
	jlj::util::List<jlj::util::Map<String, String> > evaluateToAttributesMapList(const String& expression, Document) throw (Exception);

};

//********************************************************************

StrongReferenceCommonDeclaration(XPath, ExportedByJLJxml);

//********************************************************************

NAMESPACE_END(spi)
NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************
#endif
