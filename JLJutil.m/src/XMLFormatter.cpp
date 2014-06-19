// XMLFormatter.cpp

//********************************************************************

#ifndef XMLFormatter_cpp
#define XMLFormatter_cpp

//********************************************************************

#include <jlj/util/logging/XMLFormatter.h>
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

XMLFormatterI::~XMLFormatterI() 
{}

//********************************************************************

XMLFormatterI::XMLFormatterI()
{}

//********************************************************************

// sample from java.util.logging

#if 0
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE log SYSTEM "logger.dtd">
<log>
<record>
<date>2000-08-23 19:21:05</date>
<millis>967083665789</millis>
<sequence>1256</sequence>
<logger>kgh.test.fred</logger>
<level>INFO</level>
<class>kgh.test.XMLTest</class>
<method>writeLog</method>
<thread>10</thread>
<message>Hello world!</message>
</record>
</log>
#endif

//********************************************************************

String XMLFormatterI::format(LogRecord logrecord) const
{
	Date date = new DateI(logrecord->getMillis());
	StringBuffer buf;
	buf->append(L"<record>"); buf->append(EOL);
	buf->append(L"  <date>");
	buf->append(date->toString());
	buf->append(L"</date>"); buf->append(EOL);
	buf->append(L"  <millis>");
	buf->append((pt::large) date->getTime());
	buf->append(L"</millis>"); buf->append(EOL);
	buf->append(L"  <sequence>");
	buf->append(logrecord->getSequenceNumber());
	buf->append(L"</sequence>"); buf->append(EOL);
	buf->append(L"  <logger>");
	buf->append(logrecord->getLoggerName());
	buf->append(L"</logger>"); buf->append(EOL);
	buf->append(L"  <level>");
	buf->append(logrecord->getLevel()->getName());
	buf->append(L"</level>"); buf->append(EOL);
	buf->append(L"  <class>");
	buf->append(logrecord->getSourceClassName());
	buf->append(L"</class>"); buf->append(EOL);
	buf->append(L"  <method>");
	buf->append(logrecord->getSourceMethodName());
	buf->append(L"</method>"); buf->append(EOL);
	buf->append(L"  <thread>");
	buf->append(L"</thread>"); buf->append(EOL);
	buf->append(L"  <message>");
	buf->append(logrecord->getMessage());
	buf->append(L"</message>"); buf->append(EOL);
	buf->append(L"</record>"); buf->append(EOL);
	return buf->toString();
}

//********************************************************************

String XMLFormatterI::formatMessage(LogRecord logrecord) const
{
	return logrecord->getMessage();
}

//********************************************************************

String XMLFormatterI::getHead(Handler) const
{
	return L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<log>\n";


}

//********************************************************************

String XMLFormatterI::getTail(Handler) const
{
	return L"</log>";
}

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
