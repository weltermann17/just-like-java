// Inet4Address.cpp

//********************************************************************

#ifndef jlj_net_Inet4Address_cpp
#define jlj_net_Inet4Address_cpp

//********************************************************************

#include <jlj/net/Inet4Address.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

InetAddressI::~InetAddressI() 
{}

//********************************************************************

InetAddressI::InetAddressI() 
{}

//********************************************************************

InetAddress::InetAddress() 
{}

//********************************************************************

InetAddress::InetAddress(InetAddressI* p) 
: StrongReference<InetAddressI>(p)
{}

//********************************************************************

InetAddress InetAddress::getLocalHost() throw (UnknownHostException)
{
	static InetAddress localhost;
	if (!localhost)
	{
#if defined(WIN32)
		localhost = new Inet4AddressI(127, 0, 0, 1);
#else
		char hostname[2000];
		if (0 == gethostname(hostname, 2000))
		{
			localhost = new Inet4AddressI(String(hostname, strlen(hostname)));
			int a, b, c, d;
			localhost->getAddress(a, b, c, d);
			localhost = new Inet4AddressI(a, b, c, d);
		}
		else
		{
			localhost = new Inet4AddressI(127, 0, 0, 1);
		}
#endif
	}
	return localhost;
}

//********************************************************************

InetAddress InetAddress::getByAddress(int a, int b, int c, int d) throw (UnknownHostException)
{
	return new Inet4AddressI(a, b, c, d);
}

//********************************************************************

InetAddress InetAddress::getByName(const String& host) throw (UnknownHostException)
{
	return new Inet4AddressI(host);
}

//********************************************************************
//********************************************************************

pt::rwlock& rwlock()
{
	static pt::rwlock* rwlock_ = new pt::rwlock();
	return *rwlock_;
}

//********************************************************************

Inet4AddressI::~Inet4AddressI() 
{}

//********************************************************************

Inet4AddressI::Inet4AddressI(const String& host) throw (UnknownHostException)
{
	pt::scopewrite scope(rwlock());
	try 
	{
		ipaddress = pt::phostbyname(constcharptr(host));
		if (pt::ipnone == ipaddress) throw UnknownHostException(WITHDETAILS(host));
	} 
	catch (...)
	{
		throw UnknownHostException(WITHDETAILS(host));
	}
}

//********************************************************************

Inet4AddressI::Inet4AddressI(int a, int b, int c, int d) throw (UnknownHostException)
: ipaddress(a, b, c, d)
{
	// check for wildcard address, if not test the address
	if (!(255 == d || 255 == c || 255 == b || 255 == a))
	{
		pt::string host = "";
		bool resolved = false;
		pt::scopewrite scope(rwlock());
		try 
		{
			host = pt::phostbyaddr(ipaddress);
			resolved = 0 < pt::length(host);
		}
		catch (const pt::estream* e) 
		{
			delete e;
		}
		if (!resolved) 
		{
			char buf[255];
			sprintf(buf, "%d.%d.%d.%d", a, b, c, d);
			host = buf;
		}
	}
}

//********************************************************************

bool Inet4AddressI::equals(const Object& object) const
{
	InetAddress b;
	if (object->instanceOf(b))
	{
		return getHostAddress()->equals(b->getHostAddress());
	}
	else
	{
		return false;
	}
}

//********************************************************************

int Inet4AddressI::hashCode() const
{
	return getHostAddress()->hashCode();
}

//********************************************************************

String Inet4AddressI::toString() const
{
	StringBuffer buffer;
	buffer->append(L"<Inet4Address hostaddress=\"");
	buffer->append(getHostAddress());
	buffer->append(L"\" hostname=\"");
	buffer->append(getHostName());
	buffer->append(L"\" canonicalhostname=\"");
	buffer->append(getCanonicalHostName());
	buffer->append(L"\" />");
	return String(buffer);
}

//********************************************************************

void Inet4AddressI::getAddress(int& a, int& b, int& c, int& d) const
{
	a = ipaddress[0];
	b = ipaddress[1];
	c = ipaddress[2];
	d = ipaddress[3];
}

//********************************************************************

String Inet4AddressI::getCanonicalHostName() const
{
	return getHostName();
}

//********************************************************************

String Inet4AddressI::getHostAddress() const
{
	pt::string result = pt::iptostring(ipaddress);
	return String(result, pt::length(result));
}

//********************************************************************

String Inet4AddressI::getHostName() const
{
	pt::string host = "";
	bool resolved = false;
	pt::scopewrite scope(rwlock());
	try 
	{
		host = pt::phostbyaddr(ipaddress);
		resolved = 0 < pt::length(host);
	}
	catch (const pt::estream* e) 
	{
		delete e;
	}
	return resolved ? String(host, pt::length(host)) : getHostAddress();
}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
