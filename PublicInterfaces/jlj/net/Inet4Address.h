// InetAddress.h

//********************************************************************

#ifndef jlj_net_Inet4Address_h
#define jlj_net_Inet4Address_h

//********************************************************************

#include <jlj/net/InetAddress.h>

//********************************************************************

#include <jlj/core/ptypes/pinet.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet Inet4AddressI
    : public InetAddressI
{

protected:
    ~Inet4AddressI();
    friend class TypedReference<Inet4AddressI>;

public:
    Inet4AddressI(const String&) throw (UnknownHostException); 
    Inet4AddressI(int, int, int, int) throw (UnknownHostException); 

public:
    bool equals(const Object&) const;
    int hashCode() const;
    String toString() const;
    void getAddress(int&, int&, int&, int&) const;
    String getCanonicalHostName() const;
    String getHostAddress() const;
    String getHostName() const;

private:
    mutable pt::ipaddress ipaddress;

};

//********************************************************************

StrongReferenceCommonDeclaration(Inet4Address, ExportedByJLJnet);

//********************************************************************


NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
