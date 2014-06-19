// jlj_util_logging_Handler.h

//********************************************************************

#ifndef jlj_util_logging_Handler
#define jlj_util_logging_Handler

//********************************************************************
   
#include <jlj/util/logging/Level.h>
#include <jlj/util/logging/LogRecord.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

struct ExportedByJLJutil Formatter;

//********************************************************************

class ExportedByJLJutil HandlerI 
	: public ObjectI
{
public:	
	~HandlerI();
	HandlerI();

public:
	virtual void close() = 0;
	virtual void flush() = 0;
	virtual Formatter getFormatter() const = 0;
	virtual Level getLevel() const = 0;
	virtual bool isLoggable(const Level&) const = 0;
	virtual void publish(LogRecord) = 0;
	virtual void setFormatter(const Formatter&) = 0;
	virtual void setLevel(const Level&) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Handler, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
