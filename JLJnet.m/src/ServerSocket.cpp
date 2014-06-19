// ServerSocket.cpp

//********************************************************************

#ifndef jlj_net_ServerSocket_cpp
#define jlj_net_ServerSocket_cpp

//********************************************************************

#include <jlj/net/ServerSocket.h>
#include <jlj/net/Socket.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

ServerSocketI::~ServerSocketI() throw (IOException) 
{
	close();
}

//********************************************************************

ServerSocketI::ServerSocketI(int port) throw (IOException) 
: socketaddress(new InetSocketAddressI(InetAddress::getLocalHost(), port))
{
	init();
}

//********************************************************************

ServerSocketI::ServerSocketI(int port,
							 int backlog) throw (IOException)							 
							 : socketaddress(new InetSocketAddressI(InetAddress::getLocalHost(), port))
{
	init();
}

//********************************************************************

ServerSocketI::ServerSocketI(int port,
							 int backlog,
							 const InetAddress& bindaddress) throw (IOException)
							 : socketaddress(new InetSocketAddressI(bindaddress, port))
{
	init();
}

//******************************************************************** 

void ServerSocketI::init() throw (IOException)
{
	bound = false;
	bindnumber = 0;
	timeout = -1; // infinite
	receivebuffersize = -1; // not set	
	bind(socketaddress);
}

//******************************************************************** 

Socket ServerSocketI::accept() throw (IOException)
{
	try
	{
		bool connected = server.poll(bindnumber, timeout);

		if (connected)
		{
			pt::ipstream* socket = new pt::ipstream();
			socket->set_bufsize(SocketI::DEFAULTBUFFERSIZE);

			bool active = server.serve(*socket, bindnumber);

			if (active)
			{
				return new SocketI(socket);
			}
		}
		else
		{
			throw IOException(WITHDETAILS(L"ServerSocketI::accept() failed."));
		}

		throw IOException(WITHDETAILS(L"Activating client connection failed."));
	}
	catch (const Exception& e) 
	{
		throw IOException(WITHDETAILS(e->toString()));
	}
	catch (pt::estream* e)
	{
		String msg(e->get_message(), pt::length(e->get_message()));
		delete e;
		throw IOException(WITHDETAILS(L"ServerSocketI::accept() : underlying system error."));
	}
}

//******************************************************************** 

bool ServerSocketI::bind(const SocketAddress& address) throw (IOException)
{
	return bind(address, 0);
}

//******************************************************************** 

bool ServerSocketI::bind(const SocketAddress& socketaddress, int) throw (IOException) 
{
	int a, b, c, d;
	this->socketaddress = socketaddress;
	socketaddress->getAddress()->getAddress(a, b, c, d);

	try
	{
		bindnumber = server.bind(pt::ipaddress(a, b, c, d), socketaddress->getPort());

		server.poll(bindnumber, 0); // testing, will throw if bind failed

		bound = true;			 
		return bound;
	}
	catch (pt::estream* e)
	{
		bound = false;
		bindnumber = 0;
		String msg(e->get_message(), pt::length(e->get_message()));
		delete e;
		throw IOException(WITHDETAILS(msg));
	}			
}

//******************************************************************** 

void ServerSocketI::close() throw (IOException) 
{
	if (!isClosed())
	{
		try
		{
			server.clear(); // calls close, too
			bound = false;
			bindnumber = 0;
		}
		catch (pt::estream* e)
		{
			bound = false;
			bindnumber = 0;
			String msg(e->get_message(), pt::length(e->get_message()));
			delete e;
			throw IOException(WITHDETAILS(msg));
		}			
	}
}

//******************************************************************** 

InetAddress ServerSocketI::getInetAddress() const 
{
	return socketaddress->getAddress();
}

//******************************************************************** 

SocketAddress ServerSocketI::getLocalSocketAddress() const 
{
	return socketaddress;
}

//******************************************************************** 

int ServerSocketI::getLocalPort() const 
{
	return socketaddress->getPort();
}

//******************************************************************** 

int ServerSocketI::getReceiveBufferSize() const throw (SocketException)  
{
	if (isClosed()) throw SocketException(WITHDETAILS(L"Already closed."));
	return receivebuffersize;
}

//******************************************************************** 

int ServerSocketI::getSoTimeout() const throw (SocketException)  
{
	if (isClosed()) throw SocketException(WITHDETAILS(L"Already closed."));
	return timeout;
}

//******************************************************************** 

bool ServerSocketI::isBound() const 
{
	return bound;
}

//******************************************************************** 

bool ServerSocketI::isClosed() const 
{
	return !isBound();
}

//******************************************************************** 

void ServerSocketI::setReceiveBufferSize(int size) throw (SocketException, IllegalArgumentException) 
{
	if (isClosed()) throw SocketException(WITHDETAILS(L"Already closed."));
	if (0 >= size || size > 64*1024) throw IllegalArgumentException(WITHDETAILS(L"Size must be between >0 and 64k."));
	receivebuffersize = size;
}

//******************************************************************** 

void ServerSocketI::setSoTimeout(int t) throw (SocketException, IllegalArgumentException)
{
	if (isClosed()) throw SocketException(WITHDETAILS(L"Already closed."));
	if (0 > t) throw IllegalArgumentException(WITHDETAILS(L"Timeout must be >= 0."));
	timeout = t;
}

//******************************************************************** 

bool ServerSocketI::getReuseAddress() const throw (SocketException) 
{
	if (isClosed()) throw SocketException(WITHDETAILS(L"Already closed."));
	return false;
}

//******************************************************************** 

void ServerSocketI::setReuseAddress(bool) throw (SocketException) 
{
	if (isClosed()) throw SocketException(WITHDETAILS(L"Already closed."));
}

//******************************************************************** 

pt::pintptr ServerSocketI::getSocketHandle() throw (SocketException)
{
	if (isClosed()) throw SocketException(WITHDETAILS(L"Already closed."));

	int n = server.get_addrcount();

	if (0 < n)
	{
		const pt::ipbindinfo& ipbindinfo = server.get_addr(0);
		return ipbindinfo.handle;
	}
	else
	{
		return pt::invhandle;
	}
}

//********************************************************************
//********************************************************************

ServerSocket::ServerSocket(int port) throw (IOException)
: StrongReference<ServerSocketI>(new ServerSocketI(port))
{}

//********************************************************************

ServerSocket::ServerSocket(int port, int backlog) throw (IOException)
: StrongReference<ServerSocketI>(new ServerSocketI(port, backlog))
{}

//********************************************************************

ServerSocket::ServerSocket(int port, int backlog, const InetAddress& address) throw (IOException)
: StrongReference<ServerSocketI>(new ServerSocketI(port, backlog, address))
{}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
