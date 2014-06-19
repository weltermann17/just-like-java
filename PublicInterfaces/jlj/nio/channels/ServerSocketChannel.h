// ServerSocketChannel.h

//********************************************************************

#ifndef jlj_nio_ServerSocketChannel_h
#define jlj_nio_ServerSocketChannel_h

//********************************************************************

#include <jlj/nio/channels/SelectableChannel.h>
#include <jlj/nio/channels/SocketChannel.h>
#include <jlj/net/ServerSocket.h>
using namespace jlj::net;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

struct ExportedByJLJnio ServerSocketChannel;

//********************************************************************

class ExportedByJLJnio ServerSocketChannelI
	: public SelectableChannelI
{
public:
	~ServerSocketChannelI();
	
private:
	ServerSocketChannelI(ServerSocket); 
	friend struct ExportedByJLJnio ServerSocketChannel; 

public:
	SocketChannel accept() throw (IOException);
	void close() throw (IOException);
	bool isOpen() const;
	ServerSocket socket() ;
	int validOps();

private:
	ServerSocket socket_;

};

//********************************************************************

struct ExportedByJLJnio ServerSocketChannel : StrongReference<ServerSocketChannelI> 
{    
	StrongReferenceCommonDerivateImplementation(ServerSocketChannel);

	static ServerSocketChannel open(
		int port, int backlog, const InetAddress& bindaddress) throw (IOException);
};

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
