// SocketChannel.h

//********************************************************************

#ifndef jlj_nio_SocketChannel_h
#define jlj_nio_SocketChannel_h

//********************************************************************

#include <jlj/nio/channels/SelectableChannel.h>
#include <jlj/net/Socket.h>
using namespace jlj::net;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

struct ExportedByJLJnio SocketChannel;
class ExportedByJLJnio ServerSocketChannelI; 

//********************************************************************

class ExportedByJLJnio SocketChannelI
	: public SelectableChannelI
{
public:
	~SocketChannelI();

private:
	SocketChannelI(Socket); 
	friend struct ExportedByJLJnio SocketChannel; 
	friend class ExportedByJLJnio ServerSocketChannelI; 

public:
	void close() throw (IOException);
	bool isOpen() const;
	Socket socket();
	int validOps();

private:
	Socket socket_;

};

//********************************************************************

struct ExportedByJLJnio SocketChannel : StrongReference<SocketChannelI> 
{    
	StrongReferenceCommonDerivateImplementation(SocketChannel);

	static SocketChannel open(SocketAddress, int timeout = -1) throw (IOException);
};

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif


