//Exceptions.h

//********************************************************************

#ifndef jlj_xml_Exceptions_h
#define jlj_xml_Exceptions_h

//********************************************************************

#include <jlj/lang/Throwable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)

//********************************************************************

DeriveException(ExportedByJLJxml, SAXException, jlj::lang::Exception);

//********************************************************************

NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************
//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(xpath)

//********************************************************************

DeriveException(ExportedByJLJxml, XPathExpressionException, jlj::lang::Exception);

//********************************************************************

NAMESPACE_END(xpath)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************
//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(parsers)

//********************************************************************

DeriveException(ExportedByJLJxml, ParserConfigurationException, jlj::lang::Exception);

//********************************************************************

NAMESPACE_END(parsers)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************
#endif

// eof
