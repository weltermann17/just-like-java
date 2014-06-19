// ReactorTransportationLayer.h

//********************************************************************

#ifndef jmstools_implementation_reactor_ReactorTransportationLayer_h
#define jmstools_implementation_reactor_ReactorTransportationLayer_h

//********************************************************************

#include <jmstools/implementation/reactor/TransportationLayer.h>
#include <jlj/nio/reactor/Reactor.h>

using namespace jlj::nio::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation ReactorTransportationLayerI 
	: public TransportationLayerI
{
protected:
	~ReactorTransportationLayerI();
	friend class TypedReference<ReactorTransportationLayerI>;

public:
	ReactorTransportationLayerI(ExecutorService<int>, const String& hostname, int port, int timeout, int shutdowntimeout);

public: 
	void initialize() throw (IOException);
	void finalize() throw (IOException);
	void send(Anything) throw (IOException);
	Anything receive(int timeout) throw (IOException);
	bool hasProvider() const;

private:
	Reactor reactor;
	String connectionid;
	Lock lock;
	int shutdowntimeout;
	String hostname;
	int port;
	ExecutorService<int> threadpool;

};

//********************************************************************

struct ExportedByJMSreactorimplementation ReactorTransportationLayer : StrongReference<ReactorTransportationLayerI>
{
	StrongReferenceCommonDerivateImplementation(ReactorTransportationLayer);

	//static const String NAME;

};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
