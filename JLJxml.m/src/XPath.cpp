//XPath.cpp

//********************************************************************

#ifndef spi_XPath_cpp
#define spi_XPath_cpp

//********************************************************************

#include <jlj/xml/xpath/spi/XPath.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/ArrayList.h>
#include <jlj/util/LinkedHashMap.h>
using namespace jlj::util;
#include <locale.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)
NAMESPACE_BEGIN(spi)

//********************************************************************

XPathI::~XPathI()
{}

//********************************************************************

XPathI::XPathI()
{}

//********************************************************************

String XPathI::evaluateToString(const String& text, 
								Document document) throw (Exception)
{	
	if (0 == document) throw NullPointerException();

	try
	{
		AutoRelease<xercesc::DOMXPathExpression> expression(
			document->createExpression(X(constcharptr(text)), 0));

		AutoRelease<xercesc::DOMXPathResult> result(
			expression->evaluate(document, xercesc::DOMXPathResult::ITERATOR_RESULT_TYPE, 0));

		if (result->iterateNext())
		{
			return String(result->getNodeValue()->getTextContent(), 0);
		}
		else
		{
			throw XPathExpressionException(WITHDETAILS(L"Not found : " + text));
		}
	}
	catch (const xercesc::DOMException& e)
	{
		throw XPathExpressionException(WITHDETAILS(String(e.getMessage(), 0)));
	}
}

//********************************************************************

jlj::util::List<String> XPathI::evaluateToStringList(const String& text, 
													 Document document) throw (Exception)
{
	if (0 == document) throw NullPointerException();

	try
	{
		AutoRelease<xercesc::DOMXPathExpression> expression(
			document->createExpression(X(constcharptr(text)), 0));

		AutoRelease<xercesc::DOMXPathResult> result(
			expression->evaluate(document, xercesc::DOMXPathResult::ITERATOR_RESULT_TYPE, 0));

		jlj::util::List<String> list = new LinkedListI<String>();
		while (result->iterateNext())
		{
			list->add(String(result->getNodeValue()->getTextContent(), 0));
		}
		if (0 == list->size()) throw XPathExpressionException(WITHDETAILS(L"Not found : " + text));
		return list;
	}
	catch (const xercesc::DOMException& e)
	{
		throw XPathExpressionException(WITHDETAILS(String(e.getMessage(), 0)));
	}
}

//********************************************************************

Node XPathI::evaluateToNode(const String& text, 
							Document document) throw (Exception)
{	
	if (0 == document) throw NullPointerException();

	try
	{
		AutoRelease<xercesc::DOMXPathExpression> expression(
			document->createExpression(X(constcharptr(text)), 0));

		AutoRelease<xercesc::DOMXPathResult> result(
			expression->evaluate(document, xercesc::DOMXPathResult::ITERATOR_RESULT_TYPE, 0));

		if (result->iterateNext())
		{
			return result->getNodeValue();
		}
		else
		{
			throw XPathExpressionException(WITHDETAILS(L"Not found : " + text));
		}
	}
	catch (const xercesc::DOMException& e)
	{
		throw XPathExpressionException(WITHDETAILS(String(e.getMessage(), 0)));
	}
}

//********************************************************************

List<Node> XPathI::evaluateToNodeList(const String& text, 
									  Document document) throw (Exception)
{
	if (0 == document) throw NullPointerException();

	try
	{
		AutoRelease<xercesc::DOMXPathExpression> expression(
			document->createExpression(X(constcharptr(text)), 0));

		AutoRelease<xercesc::DOMXPathResult> result(
			expression->evaluate(document, xercesc::DOMXPathResult::ITERATOR_RESULT_TYPE, 0));

		List<Node> list = new ArrayListI<Node>(512);
		while (result->iterateNext())
		{
			list->add(result->getNodeValue());
		}
		if (0 == list->size()) throw XPathExpressionException(WITHDETAILS(L"Not found : " + text));

		return list;
	}
	catch (const xercesc::DOMException& e)														    
	{
		throw XPathExpressionException(WITHDETAILS(String(e.getMessage(), 0)));
	}
}

//********************************************************************

Map<String, String> XPathI::evaluateToAttributesMap(const String& text, 
													Document document) throw (Exception)
{
	Node node = evaluateToNode(text, document);
	Map<String, String> result = new LinkedHashMapI<String, String>();
	try
	{
		if (node->hasAttributes())
		{
			xercesc::DOMNamedNodeMap* namednodemap = node->getAttributes();
			for (XMLSize_t i = 0; i < namednodemap->getLength(); ++i)
			{
				xercesc::DOMAttr* item = (xercesc::DOMAttr*) namednodemap->item(i);
				result->put(String(item->getName(), 0), String(item->getValue(), 0));
			}
		}
		return result;
	}
	catch (const xercesc::DOMException& e)
	{
		throw XPathExpressionException(WITHDETAILS(String(e.getMessage(), 0)));
	}
}

//********************************************************************

List<Map<String, String> > XPathI::evaluateToAttributesMapList(const String& text, 
															   Document document) throw (Exception)
{
	List<Node> nodelist = evaluateToNodeList(text, document);
	List<Map<String, String> > result = new ArrayListI<Map<String, String> >(nodelist->size());
	Iterator<Node> n = nodelist->iterator();
	while (n->hasNext())
	{
		Node node = n->next();
		Map<String, String> attr = new LinkedHashMapI<String, String>();
		try
		{
			if (node->hasAttributes())
			{
				xercesc::DOMNamedNodeMap* namednodemap = node->getAttributes();
				for (XMLSize_t i = 0; i < namednodemap->getLength(); ++i)
				{
					xercesc::DOMAttr* item = (xercesc::DOMAttr*) namednodemap->item(i);
					attr->put(String(item->getName(), 0), String(item->getValue(), 0));
				}
			}
			result->add(attr);
		}
		catch (const xercesc::DOMException& e)
		{
			throw XPathExpressionException(WITHDETAILS(String(e.getMessage(), 0)));
		}
	}
	return result;
}

//********************************************************************

NAMESPACE_END(spi)
NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************
//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)

//********************************************************************

XPathI::~XPathI()
{}

//********************************************************************

XPathI::XPathI()
{}

//********************************************************************

NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************
#endif

