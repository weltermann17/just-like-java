// ControlHandler.h

//********************************************************************

#ifndef jmstools_implementation_reactor_ControlHandler_h
#define jmstools_implementation_reactor_ControlHandler_h

//********************************************************************

#include <jmstools/implementation/reactor/Handler.h>
#include <jmstools/implementation/reactor/TransportationLayer.h>
#include <jlj/util/Anything.h>

using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation ControlHandlerI
	: public HandlerI
{
public:
	ControlHandlerI(TransportationLayer, Anything);

public:
	void handle() throw (Exception);

protected:
	~ControlHandlerI();
	friend class TypedReference<ControlHandlerI>;

private:
	TransportationLayer transportationlayer;
	Anything anything;
};

//********************************************************************

StrongReferenceCommonDeclaration(ControlHandler, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)


#endif

// eof