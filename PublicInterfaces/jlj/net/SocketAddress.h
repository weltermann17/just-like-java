// SocketAddress.h

//********************************************************************

#ifndef jlj_net_SocketAddress_h
#define jlj_net_SocketAddress_h

//********************************************************************

#include <jlj/net/InetAddress.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet SocketAddressI 
	: public jlj::lang::ObjectI
{
public:
	virtual ~SocketAddressI() = 0;
	SocketAddressI();

public:
	virtual InetAddress getAddress() const = 0;
	virtual String getHostName() const = 0;
	virtual int getPort() const = 0;
	virtual bool isUnresolved() const = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(SocketAddress, ExportedByJLJnet);

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
