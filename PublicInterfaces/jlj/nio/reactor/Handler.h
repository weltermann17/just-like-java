// Handler.h

//********************************************************************

#ifndef jlj_nio_reactor_Handler_h
#define jlj_nio_reactor_Handler_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
using namespace jlj::lang;
#include <jlj/nio/channels/SelectionKey.h>
using namespace jlj::nio::channels;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

struct ExportedByJLJnio Reactor;

//********************************************************************

class ExportedByJLJnio HandlerI 
	: public virtual ObjectI
{
public:
	virtual ~HandlerI() = 0;
	HandlerI();

	virtual void handleEvent(SelectionKey) = 0;
	virtual void registerWith(Reactor) = 0;

};

//********************************************************************

typedef ExportedByJLJnio StrongReference<HandlerI> Handler;

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
