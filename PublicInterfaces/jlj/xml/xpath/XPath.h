//XPath.h

//********************************************************************

#ifndef jlj_xml_xpath_XPath_h
#define jlj_xml_xpath_XPath_h

//********************************************************************
#include <jlj/xml/Typedef.h>
#include <jlj/xml/Exceptions.h>

#include <jlj/lang/String.h>
#include <jlj/io/Exceptions.h>
#include <jlj/util/List.h>
#include <jlj/util/Map.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)

//********************************************************************

class ExportedByJLJxml XPathI
	: public ObjectI 
{

public:
	virtual ~XPathI() = 0;
	XPathI();

public:
	virtual String evaluateToString(const String& expression, Document) throw (Exception) = 0;
	virtual jlj::util::List<String> evaluateToStringList(const String& expression, Document) throw (Exception) = 0;
	virtual Node evaluateToNode(const String& expression, Document) throw (Exception) = 0;
	virtual jlj::util::List<Node> evaluateToNodeList(const String& expression, Document) throw (Exception) = 0;
	virtual jlj::util::Map<String, String> evaluateToAttributesMap(const String& expression, Document) throw (Exception) = 0;
	virtual jlj::util::List<jlj::util::Map<String, String> > evaluateToAttributesMapList(const String& expression, Document) throw (Exception) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(XPath, ExportedByJLJxml);

//********************************************************************

NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//*************************************************************
#endif
