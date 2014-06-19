// InetSocketAddress.h

//********************************************************************

#ifndef jlj_net_InetSocketAddress_h
#define jlj_net_InetSocketAddress_h

//********************************************************************

#include <jlj/net/SocketAddress.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet InetSocketAddressI 
    : public SocketAddressI
{
protected:
    ~InetSocketAddressI();
    friend class TypedReference<InetSocketAddressI>;
    
public:
    InetSocketAddressI(const InetAddress&, int port) throw (IllegalArgumentException);
    InetSocketAddressI(int port) throw (IllegalArgumentException);
    InetSocketAddressI(const String& hostname, int port) throw (IllegalArgumentException);

public:
    bool equals(const Object&) const;
    int hashCode() const;
    String toString() const;
    InetAddress getAddress() const;
    String getHostName() const;
    int getPort() const;
    bool isUnresolved() const;

private:
    InetAddress address;
    int port;

};

//********************************************************************

StrongReferenceCommonDeclaration(InetSocketAddress, ExportedByJLJnet);

//********************************************************************


NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
