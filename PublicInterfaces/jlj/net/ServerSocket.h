// ServerSocket.h

//********************************************************************

#ifndef jlj_net_ServerSocket_h
#define jlj_net_ServerSocket_h

//********************************************************************

#include <jlj/net/Exceptions.h>
#include <jlj/net/InetSocketAddress.h>
#include <jlj/net/Inet4Address.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet Socket;

//********************************************************************

class ExportedByJLJnet ServerSocketI
	: public ObjectI
{

protected:
    ~ServerSocketI() throw (IOException);
    friend class TypedReference<ServerSocketI>;
    
public:
	ServerSocketI(int port) throw (IOException);
	ServerSocketI(int port, int backlog) throw (IOException);
	ServerSocketI(int port, int backlog, const InetAddress&) throw (IOException);
	
public:
	Socket accept() throw (IOException);
	bool bind(const SocketAddress&) throw (IOException);
	bool bind(const SocketAddress&, int backlog) throw (IOException);
	void close() throw (IOException);
	InetAddress getInetAddress() const;
	int getLocalPort() const;
	SocketAddress getLocalSocketAddress() const;
	int getReceiveBufferSize() const throw (SocketException);
	bool getReuseAddress() const throw (SocketException);
	int getSoTimeout() const throw (SocketException);
	bool isBound() const;
	bool isClosed() const;
	void setReceiveBufferSize(int) throw (SocketException, IllegalArgumentException);
	void setReuseAddress(bool) throw (SocketException);
	void setSoTimeout(int) throw (SocketException, IllegalArgumentException);

	// we need this for implementing jlj::nio::channels;
	pt::pintptr getSocketHandle() throw (SocketException);

private:
	void init() throw (IOException);

private:	
	SocketAddress socketaddress;
	pt::ipstmserver server;	
	int timeout;
	int receivebuffersize;
	bool bound;
	int bindnumber;
	
};

//********************************************************************

struct ExportedByJLJnet ServerSocket : StrongReference<ServerSocketI> 
{    
	StrongReferenceCommonDerivateImplementation(ServerSocket);
    ServerSocket(int port) throw (IOException);    
    ServerSocket(int port, int backlog) throw (IOException);    
    ServerSocket(int port, int backlog, const InetAddress& bindaddress) throw (IOException);    
};

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
