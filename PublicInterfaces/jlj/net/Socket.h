// Socket.h

//********************************************************************

#ifndef jlj_net_Socket_h
#define jlj_net_Socket_h

//********************************************************************

#include <jlj/net/Exceptions.h>
#include <jlj/net/InetSocketAddress.h>
#include <jlj/net/Inet4Address.h>
#include <jlj/io/InputStream.h>
#include <jlj/io/OutputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet SocketI 
    : public jlj::lang::ObjectI
{

protected:
    ~SocketI() throw (IOException);
    friend class TypedReference<SocketI>;

public:
    SocketI(const InetAddress&, int port, int timeout) throw (IOException); 
    SocketI(
        const InetAddress& remote, 
        int port, 
        const InetAddress& local,
        int localport) throw (IOException); 

public:
    String toString() const;

public:
    virtual bool bind(const SocketAddress&) throw (IOException);
    virtual void close() throw (IOException);
    virtual bool connect(const SocketAddress&) throw (IOException);
    virtual bool connect(const SocketAddress&, int timeout) throw (IOException);
    virtual InetAddress getInetAddress() const;
    virtual InputStream getInputStream() throw (IOException);
    virtual bool getKeepAlive() const throw (SocketException);
    virtual InetAddress getLocalAddress() const;
    virtual int getLocalPort() const;
    virtual SocketAddress getLocalSocketAddress() const;
    virtual OutputStream getOutputStream() throw (IOException);
    virtual int getPort() const;
    virtual int getReceiveBufferSize() const throw (SocketException);
    virtual SocketAddress getRemoteSocketAddress() const;
    virtual int getSendBufferSize() const throw (SocketException);
    virtual int getSoLinger() const throw (SocketException);
    virtual int getSoTimeout() const throw (SocketException);
    virtual bool getTcpNoDelay() const throw (SocketException);
    virtual int getTrafficClass() const throw (SocketException);
    virtual bool isBound() const;
    virtual bool isClosed() const;
    virtual bool isConnected() const;
    virtual bool isInputShutdown() const;
    virtual bool isOutputShutdown() const;
    virtual void setKeepAlive(bool) throw (SocketException);
    virtual void setReceiveBufferSize(int) throw (SocketException);
    virtual void setSendBufferSize(int) throw (SocketException);
    virtual void setSoLinger(bool on, int) throw (SocketException);
    virtual void setSoTimeout(int) throw (SocketException);
    virtual void setTcpNoDelay(bool) throw (SocketException);
    virtual void setTrafficClass(int) throw (SocketException);
    virtual void shutdownInput() throw (IOException);
    virtual void shutdownOutput() throw (IOException);

    // we need this for implementing jlj::nio::channels;
    pt::pintptr getSocketHandle() throw (SocketException);

protected:
    SocketI(pt::ipstream* socket) throw (IOException);
    friend class ExportedByJLJnet ServerSocketI;
    SocketI() throw (IOException);

protected:
    virtual bool open() throw (IOException);
    virtual void check() const throw (SocketException);

protected:
    SocketAddress remote;
    SocketAddress local;
    bool bound;
    int timeout;
    pt::ipstream* socket;
    InputStream inputstream;
    OutputStream outputstream;
    static const int DEFAULTBUFFERSIZE;

};

//********************************************************************

struct ExportedByJLJnet Socket : StrongReference<SocketI> 
{    
    StrongReferenceCommonDerivateImplementation(Socket);
       Socket(const InetAddress&, int port, int timeout = -1) throw (IOException);    
    Socket(const InetAddress&, int remoteport, const InetAddress&, int localport) throw (IOException);    
};

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
