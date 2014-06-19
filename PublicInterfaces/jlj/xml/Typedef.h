//Typedef.h

//******************************************************

#ifndef jlj_xml_Typedef_h
#define jlj_xml_Typedef_h

//******************************************************

#include <jlj/lang/Object.h>

//******************************************************

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xqilla/xqilla-dom3.hpp>

using namespace jlj::lang;

//******************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)

//******************************************************

typedef xercesc::DOMDocument* Document;
typedef xercesc::DOMNode* Node;
typedef xercesc::DOMNodeList* NodeList;
typedef xercesc::DOMNamedNodeMap* NamedNodeMap;

//******************************************************

NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//******************************************************

#endif
