// SimpleFormatter.cpp

//********************************************************************

#ifndef SimpleFormatter_cpp
#define SimpleFormatter_cpp

//********************************************************************

#include <jlj/util/logging/SimpleFormatter.h>
#include <jlj/lang/StringBuffer.h>
using namespace jlj::lang;
#include <jlj/util/Date.h>
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

#if defined(_WINDOWS_SOURCE)
#define EOL L"\n"
#else
#define EOL L"\n"
#endif

//********************************************************************

SimpleFormatterI::~SimpleFormatterI() 
{}

//********************************************************************

SimpleFormatterI::SimpleFormatterI()
{}

//********************************************************************

String SimpleFormatterI::format(LogRecord logrecord) const
{
	Date date = new DateI(logrecord->getMillis());
	StringBuffer time(date->toString());
	time = time->substring(11);
	StringBuffer buf;
	buf->append(logrecord->getLoggerName());
	buf->append(L" ");
	buf->append(logrecord->getSequenceNumber());
	buf->append(L" (");
	buf->append(time);
	buf->append(L") : ");
	buf->append(logrecord->getMessage());
	buf->append(EOL);
	return buf->toString();
}

//********************************************************************

String SimpleFormatterI::formatMessage(LogRecord logrecord) const
{
	return logrecord->getMessage();
}

//********************************************************************

String SimpleFormatterI::getHead(Handler) const
{
	return L"";
}

//********************************************************************

String SimpleFormatterI::getTail(Handler) const
{
	return L"";
}

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
