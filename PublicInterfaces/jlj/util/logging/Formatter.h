// jlj_util_logging_Formatter.h

//********************************************************************

#ifndef jlj_util_logging_Formatter
#define jlj_util_logging_Formatter

//********************************************************************
   
#include <jlj/util/logging/LogRecord.h>
#include <jlj/util/logging/Handler.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJutil FormatterI 
	: public ObjectI
{
public:	
	~FormatterI();
	FormatterI();

public:
	virtual String format(LogRecord) const = 0;
	virtual String formatMessage(LogRecord) const = 0;
	virtual String getHead(Handler) const = 0;
	virtual String getTail(Handler) const = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Formatter, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
