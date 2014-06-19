// TransportationLayer.h

//********************************************************************

#ifndef jmstools_implementation_reactor_TransportationLayer_h
#define jmstools_implementation_reactor_TransportationLayer_h

//********************************************************************

#include <jmstools/implementation/reactor/AbstractMessage.h>
#include <jlj/util/concurrent/BlockingQueue.h>
using namespace jlj::util::concurrent;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation TransportationLayerI 
	: public ObjectI
{
public:
	~TransportationLayerI() = 0;
	TransportationLayerI();

public: 
	virtual void initialize() throw (IOException) = 0;
	virtual void finalize() throw (IOException) = 0;
	virtual void send(Anything) throw (IOException) = 0;
	virtual Anything receive(int timeout) throw (IOException) = 0;
	virtual bool hasProvider() const = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(TransportationLayer, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
