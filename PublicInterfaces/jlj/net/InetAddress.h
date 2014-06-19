// InetAddress.h

//********************************************************************

#ifndef jlj_net_InetAddress_h
#define jlj_net_InetAddress_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
#include <jlj/lang/Throwable.h>
using namespace jlj::lang;

#include <jlj/net/Exceptions.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet InetAddressI 
	: public ObjectI
{
public:
	virtual ~InetAddressI() = 0;
	InetAddressI();

public:
	virtual void getAddress(int&, int&, int&, int&) const = 0;
	virtual String getCanonicalHostName() const = 0;
	virtual String getHostAddress() const = 0;
	virtual String getHostName() const = 0;

};

//********************************************************************

struct ExportedByJLJnet InetAddress : StrongReference<InetAddressI>
{    
	InetAddress();
	InetAddress(InetAddressI*);


	static InetAddress getLocalHost() throw (UnknownHostException); 
	static InetAddress getByAddress(int, int, int, int) throw (UnknownHostException);
	static InetAddress getByName(const String&) throw (UnknownHostException);
};

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
