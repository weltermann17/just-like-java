// Connector.h

//********************************************************************

#ifndef jlj_nio_reactor_Connector_h
#define jlj_nio_reactor_Connector_h

//********************************************************************

#include <jlj/nio/reactor/Connection.h>
#include <jlj/net/SocketAddress.h>
using namespace jlj::net;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJLJnio ConnectorI 
	: public HandlerI
{
public:
	~ConnectorI();
	ConnectorI(Reactor, int timeout);

	Connection connect(SocketAddress) throw (IOException);

	// connectionstring ::= <hostname>:<port>
	Connection connect(const String& connectionstring) throw (IOException);
	
	void setTimeout(int milliseconds);

	void handleEvent(SelectionKey);
	void registerWith(Reactor);

private:
	Reactor reactor;
	int timeout;

};

//********************************************************************

struct ExportedByJLJnio Connector : StrongReference<ConnectorI> 
{    
	StrongReferenceCommonDerivateImplementation(Connector);
	static Connector open(Reactor, int timeout = -1);
};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
