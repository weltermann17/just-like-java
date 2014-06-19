// Acceptor.h

//********************************************************************

#ifndef jlj_nio_reactor_Acceptor_h
#define jlj_nio_reactor_Acceptor_h

//********************************************************************

#include <jlj/nio/reactor/Reactor.h>
#include <jlj/net/InetAddress.h>
using namespace jlj::net;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJLJnio AcceptorI 
	: public HandlerI
{
public:
	~AcceptorI();
	
	AcceptorI(
		int port,
		int backlog,
		const InetAddress& bindaddress) throw (IOException);

	void handleEvent(SelectionKey);
	void registerWith(Reactor);

	void setTimeout(int milliseconds);

private:
	int port;
	int backlog;
	InetAddress bindaddress;
	int timeout;
	Reactor reactor;

};

//********************************************************************

struct ExportedByJLJnio Acceptor : StrongReference<AcceptorI> 
{    
	StrongReferenceCommonDerivateImplementation(Acceptor);

	static Acceptor open(
		int port,
		int backlog,
		const InetAddress& bindaddress) throw (IOException);
};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
