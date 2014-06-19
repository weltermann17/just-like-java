// jlj_util_logging_XMLFormatter.h

//********************************************************************

#ifndef jlj_util_logging_XMLFormatter
#define jlj_util_logging_XMLFormatter

//********************************************************************
   
#include <jlj/util/logging/Formatter.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJutil XMLFormatterI
	: public FormatterI
{
public:	
	~XMLFormatterI();
	XMLFormatterI();

public:
	String format(LogRecord) const;
	String formatMessage(LogRecord) const;
	String getHead(Handler) const;
	String getTail(Handler) const;

};

//********************************************************************

StrongReferenceCommonDeclaration(XMLFormatter, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
