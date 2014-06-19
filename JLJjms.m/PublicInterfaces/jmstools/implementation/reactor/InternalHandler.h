// InternalHandler.h

//********************************************************************

#ifndef jmstools_implementation_reactor_InternalHandler_h
#define jmstools_implementation_reactor_InternalHandler_h

//********************************************************************

#include <jmstools/implementation/reactor/Handler.h>
#include <jlj/util/Anything.h>

using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation InternalHandlerI
    : public HandlerI
{
protected:
    ~InternalHandlerI();
    friend class TypedReference<InternalHandlerI>;

public:
    InternalHandlerI(Anything);

public:
    void handle() throw (Exception);
	bool isHandled() const;

private:
    Anything anything;
	bool ishandled;
};

//********************************************************************

StrongReferenceCommonDeclaration(InternalHandler, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof