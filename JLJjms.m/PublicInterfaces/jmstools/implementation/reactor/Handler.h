// Handler.h

//********************************************************************

#ifndef jmstools_implementation_reactor_Handler_h
#define jmstools_implementation_reactor_Handler_h

//********************************************************************

#include <JMSreactorimplementation.h>
#include <jlj/lang/Object.h>
#include <jlj/lang/Throwable.h>

using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation HandlerI
	: public ObjectI
{

public:
	virtual ~HandlerI() = 0;
	HandlerI();

public:
	virtual void handle() throw (Exception) = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(Handler, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
