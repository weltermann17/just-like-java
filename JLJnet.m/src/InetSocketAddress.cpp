// InetSocketAddress.cpp

//********************************************************************

#ifndef jlj_net_InetSocketAddress_cpp
#define jlj_net_InetSocketAddress_cpp

//********************************************************************

#include <jlj/net/InetSocketAddress.h>
#include <jlj/net/Inet4Address.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

SocketAddressI::~SocketAddressI() 
{}

//********************************************************************

SocketAddressI::SocketAddressI() 
{}

//********************************************************************
//********************************************************************

InetSocketAddressI::~InetSocketAddressI() 
{}

//********************************************************************

InetSocketAddressI::InetSocketAddressI(const InetAddress& address, 
									   int port) throw (IllegalArgumentException) 
									   : address(address)
									   , port(port)
{
	if (0 > port || port > 64*1024) throw IllegalArgumentException(L"Invalid port.");
}

//********************************************************************

InetSocketAddressI::InetSocketAddressI(int port) throw (IllegalArgumentException) 
: address(new Inet4AddressI(0, 0, 0, 255))
, port(port)
{
	if (0 > port || port > 64*1024) throw IllegalArgumentException(L"Invalid port.");
}

//********************************************************************

InetSocketAddressI::InetSocketAddressI(const String& host, 
									   int port) throw (IllegalArgumentException) 
									   : port(port)
{
	try 
	{
		address = InetAddress::getByName(host);
	}
	catch (const Exception& e)
	{
		throw IllegalArgumentException(e->toString());
	}
	if (0 > port || port > 64*1024) throw IllegalArgumentException(L"Invalid port.");		                         
}

//********************************************************************

bool InetSocketAddressI::equals(const Object& object) const
{
	StrongReference<InetSocketAddressI> b;
	if (object->instanceOf(b))
	{
		return toString()->equals(b->toString());
	}
	else
	{
		return false;
	}
}

//********************************************************************

int InetSocketAddressI::hashCode() const
{
	return toString()->hashCode();
}

//********************************************************************

String InetSocketAddressI::toString() const
{
	StringBuffer buffer;
	buffer->append(L"<InetSocketAddress address=\"");
	buffer->append(address->toString());
	buffer->append(L"\" port=\"");
	buffer->append(getPort());
	buffer->append(L"\" />");
	return String(buffer);
}

//********************************************************************

InetAddress InetSocketAddressI::getAddress() const
{
	return address;
}

//********************************************************************

String InetSocketAddressI::getHostName() const
{
	return address->getHostName();
}

//********************************************************************

int InetSocketAddressI::getPort() const
{
	return port;
}

//********************************************************************

bool InetSocketAddressI::isUnresolved() const
{
	return address->getHostAddress()->equals(L"255.255.255.255");
}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
