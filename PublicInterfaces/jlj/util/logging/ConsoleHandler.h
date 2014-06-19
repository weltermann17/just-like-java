// jlj_util_logging_ConsoleHandler.h

//********************************************************************

#ifndef jlj_util_logging_ConsoleHandler
#define jlj_util_logging_ConsoleHandler

//********************************************************************
   
#include <jlj/util/logging/StreamHandler.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/AbstractNamedSingleton.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJutil ConsoleHandlerI
	: public StreamHandlerI
{
public:	
	~ConsoleHandlerI();
	ConsoleHandlerI();

public:
	void close();
	void publish(LogRecord);

};

//********************************************************************

struct ExportedByJLJutil ConsoleHandler 
	: StrongReference<ConsoleHandlerI>
	, AbstractNamedSingleton<ConsoleHandler, ConsoleHandlerI>
{
	StrongReferenceCommonDerivateImplementation(ConsoleHandler);
};

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
