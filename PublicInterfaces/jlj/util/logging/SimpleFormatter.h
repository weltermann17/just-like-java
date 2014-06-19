// jlj_util_logging_SimpleFormatter.h

//********************************************************************

#ifndef jlj_util_logging_SimpleFormatter
#define jlj_util_logging_SimpleFormatter

//********************************************************************
   
#include <jlj/util/logging/Formatter.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJutil SimpleFormatterI
	: public FormatterI
{
public:	
	~SimpleFormatterI();
	SimpleFormatterI();

public:
	String format(LogRecord) const;
	String formatMessage(LogRecord) const;
	String getHead(Handler) const;
	String getTail(Handler) const;

};

//********************************************************************

StrongReferenceCommonDeclaration(SimpleFormatter, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
