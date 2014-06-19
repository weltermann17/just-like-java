//OutputFormat.cpp

//**********************************************************************

#ifndef jlj_xml_serialize_OutputFormat_cpp
#define jlj_xml_serialize_OutputFormat_cpp

//**********************************************************************

#include <jlj/xml/serialize/OutputFormat.h>

//**********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(serialize)

//**********************************************************************

OutputFormatI::~OutputFormatI()
{}

//**********************************************************************

OutputFormatI::OutputFormatI()
: encoding(L"UTF-8")
, lineSeparator(L"\n")
, prettyPrint(true)
{}

//**********************************************************************

String OutputFormatI::getEncoding() const
{
    return encoding;
}

//**********************************************************************

void OutputFormatI::setEncoding(const String& encoding)
{
    this->encoding = encoding;
}

//**********************************************************************

String OutputFormatI::getLineSeparator() const
{
    return lineSeparator;
}

//**********************************************************************

void OutputFormatI::setLineSeparator(const String& lineSeparator)
{
    this->lineSeparator = lineSeparator;
}

//**********************************************************************

bool OutputFormatI::getPrettyPrint() const
{
    return prettyPrint;
}

//**********************************************************************

NAMESPACE_END(serialize)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//**********************************************************************

#endif