// Socket.cpp

//********************************************************************

#ifndef jlj_net_Socket_cpp
#define jlj_net_Socket_cpp

//********************************************************************

#include <jlj/net/Socket.h>
#include <jlj/net/SocketInputStream.h>
#include <jlj/net/SocketOutputStream.h>

#include <jlj/util/logging/Logger.h>
using jlj::util::logging::Logger;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

const int SocketI::DEFAULTBUFFERSIZE = 35*1460;

//********************************************************************

SocketI::~SocketI() throw (IOException) 
{
	close();
}

//********************************************************************

SocketI::SocketI(const InetAddress& remote, 
				 int port, 
				 int timeout) throw (IOException) 
				 : remote(new InetSocketAddressI(remote, port))
				 , socket(new pt::ipstream())
				 , timeout(-1)
				 , bound(false)
{
	open();
}

//*******************************************************************

SocketI::SocketI(const InetAddress& remote, 
				 int remoteport,
				 const InetAddress& local, 
				 int localport) throw (IOException) 
				 : remote(new InetSocketAddressI(remote, remoteport))
				 , local(new InetSocketAddressI(local, localport))
				 , socket(new pt::ipstream())
				 , timeout(-1)
				 , bound(false)
{
	open();
}

//********************************************************************

SocketI::SocketI(pt::ipstream* socket) throw (IOException) 
: socket(socket)
, timeout(-1)
{
	try 
	{
		if (0 == socket) 
		{
			throw IOException(WITHDETAILS(L"SocketI::SocketI(pt::ipstream* socket) : NPE."));	
		}
		bool hostfound = false;
		String hostname;
		try 
		{
			String hostname(pt::pconst(socket->get_host()), pt::length(socket->get_host()));
			hostfound = true;
		}
		catch (const pt::estream* e) 
		{
			delete e;
		}
		if (hostfound)
		{
			try 
			{
				remote = new InetSocketAddressI(hostname, socket->get_port());
			}
			catch (const IllegalArgumentException&)
			{
				try 
				{
					pt::ipaddress loc = socket->get_ip();
					int a, b, c, d;
					a = loc[0]; b = loc[1]; c = loc[2]; d = loc[3]; 
					remote = new InetSocketAddressI(InetAddress::getByAddress(a, b, c, d), socket->get_port());
				}
				catch (...)
				{
					throw IOException(WITHDETAILS(L"Cannot resolve remote address."));	                                
				}
			}
		} 
		else
		{
			try 
			{
				pt::ipaddress loc = socket->get_ip();
				int a, b, c, d;
				a = loc[0]; b = loc[1]; c = loc[2]; d = loc[3]; 
				remote = new InetSocketAddressI(InetAddress::getByAddress(a, b, c, d), socket->get_port());
			}
			catch (...)
			{
				throw IOException(WITHDETAILS(L"Cannot resolve remote address."));	                                
			}
		}
		pt::ipaddress loc = socket->get_myip();
		int a, b, c, d;
		a = loc[0]; b = loc[1]; c = loc[2]; d = loc[3]; 
		local = new InetSocketAddressI(InetAddress::getByAddress(a, b, c, d), socket->get_myport());
		bound = true;
	}
	catch (const Exception& e)
	{
		throw e;
	}
	catch (...)
	{
		throw IOException(WITHDETAILS(L"SocketI::SocketI(pt::ipstream* socket) : Underlying system error."));	                                
	}
}

//********************************************************************

SocketI::SocketI() throw (IOException) 
: socket(0)
, timeout(-1)
, bound(false)
{}

//********************************************************************

bool SocketI::open() throw (IOException) 
{
	pt::string hostaddress(constcharptr(remote->getAddress()->getHostAddress()));

	try
	{
		socket->set_host(hostaddress);
		socket->set_port(remote->getPort());
		socket->set_bufsize(DEFAULTBUFFERSIZE);

		socket->open();
	}
	catch (pt::estream* e)
	{
		String msg(e->get_message(), pt::length(e->get_message()));
		delete e;
		delete socket;
		socket = 0;
		throw IOException(WITHDETAILS(msg));
	}

	if (pt::IO_OPENED == socket->get_status())
	{
		pt::ipaddress loc = socket->get_myip();
		int a, b, c, d;
		a = loc[0]; b = loc[1]; c = loc[2]; d = loc[3]; 
		local = new InetSocketAddressI(InetAddress::getByAddress(a, b, c, d), socket->get_myport());
		bound = true;
		return true;
	}
	else
	{
		socket->cancel();		
		delete socket;
		socket = 0;
		return false;
	}
}

//******************************************************************** 

bool SocketI::bind(const SocketAddress&) throw (IOException) 
{
	check();
	return bound;
}

//******************************************************************** 

void SocketI::close() throw (IOException) 
{
	if (!isClosed())
	{
		check();

		shutdownInput();
		shutdownOutput();

		socket->close();
		delete socket;
		socket = 0;
	}
}

//******************************************************************** 

bool SocketI::connect(const SocketAddress& remote) throw (IOException) 
{
	return connect(remote, -1);
}

//******************************************************************** 

bool SocketI::connect(const SocketAddress& remote, int timeout) throw (IOException) 
{
	close();
	this->remote = remote;
	this->timeout = timeout;
	return open();
}

//******************************************************************** 

InetAddress SocketI::getInetAddress() const 
{
	return remote->getAddress();
}

//******************************************************************** 

InputStream SocketI::getInputStream() throw (IOException)
{
	check();
	if (!inputstream) inputstream = new SocketInputStreamI(socket, timeout);
	return inputstream;
}

//******************************************************************** 

bool SocketI::getKeepAlive() const throw (SocketException)
{
	check();
	return false;
}

//******************************************************************** 

InetAddress SocketI::getLocalAddress() const 
{
	return local->getAddress();
}

//******************************************************************** 

int SocketI::getLocalPort() const 
{
	return local->getPort();
}

//******************************************************************** 

SocketAddress SocketI::getLocalSocketAddress() const 
{
	return local;
}

//******************************************************************** 

OutputStream SocketI::getOutputStream() throw (IOException) 
{
	check();
	if (!outputstream) outputstream = new SocketOutputStreamI(socket);
	return outputstream;
}

//******************************************************************** 

int SocketI::getPort() const 
{
	return remote->getPort();
}

//******************************************************************** 

int SocketI::getReceiveBufferSize() const throw (SocketException)
{
	check();
	return socket->get_bufsize();
}

//********************************************************************

SocketAddress SocketI::getRemoteSocketAddress() const 
{
	return remote;
}

//******************************************************************** 

int SocketI::getSendBufferSize() const throw (SocketException) 
{
	check();
	return getReceiveBufferSize();
}

//******************************************************************** 

int SocketI::getSoLinger() const throw (SocketException) 
{
	check();
	return 0;
}

//******************************************************************** 

int SocketI::getSoTimeout() const throw (SocketException) 
{
	check();
	return timeout;
}

//******************************************************************** 

bool SocketI::getTcpNoDelay() const throw (SocketException) 
{
	check();
	return false;
}

//******************************************************************** 

int SocketI::getTrafficClass() const throw (SocketException)
{
	check();
	return 0;
}

//******************************************************************** 

bool SocketI::isBound() const 
{
	return bound;
}

//******************************************************************** 

bool SocketI::isClosed() const 
{
	if (0 == socket) 
	{
		return true;
	}
	else
	{
		return pt::IO_CLOSED == socket->get_status();
	}
}

//******************************************************************** 

bool SocketI::isConnected() const 
{
	if (0 == socket) 
	{
		return false;
	}
	else
	{
		int status = socket->get_status();
		return pt::IO_CONNECTED == status
			|| pt::IO_OPENED == status
			|| pt::IO_READING == status
			|| pt::IO_WRITING == status
			|| pt::IO_EOF == status;
	}
}

//******************************************************************** 

bool SocketI::isInputShutdown() const 
{
	return !inputstream;
}

//******************************************************************** 

bool SocketI::isOutputShutdown() const 
{
	return !outputstream;
}

//******************************************************************** 

void SocketI::setKeepAlive(bool) throw (SocketException) 
{
	check();
}

//******************************************************************** 

void SocketI::setReceiveBufferSize(int size) throw (SocketException) 
{
	check();
	socket->set_bufsize(size);
}

//******************************************************************** 

void SocketI::setSendBufferSize(int size) throw (SocketException) 
{
	setReceiveBufferSize(size);
}

//******************************************************************** 

void SocketI::setSoLinger(bool, int) throw (SocketException) 
{
	check();
}

//******************************************************************** 

void SocketI::setSoTimeout(int timeout) throw (SocketException) 
{
	check();
	this->timeout = timeout;
}

//******************************************************************** 

void SocketI::setTcpNoDelay(bool) throw (SocketException)
{
	check();
}

//******************************************************************** 

void SocketI::setTrafficClass(int) throw (SocketException)
{
	check();
}

//******************************************************************** 

void SocketI::shutdownInput() throw (IOException) 
{
	check();
	if (!!inputstream) 
	{
		inputstream->close();
		inputstream.reset(0);
	}
}

//******************************************************************** 

void SocketI::shutdownOutput() throw (IOException) 
{
	check();
	if (!!outputstream)
	{
		outputstream->close();
		outputstream.reset(0);
	}
}

//******************************************************************** 

void SocketI::check() const throw (SocketException) 
{
	if (isClosed()) 
	{
		throw SocketException(WITHDETAILS(L"Already closed."));
	}
}

//******************************************************************** 

String SocketI::toString() const 
{
	StringBuffer buffer;
	buffer->append(remote->toString());
	buffer->append(L", ");
	buffer->append(local->toString());
	buffer->append(L", connected=");
	buffer->append(isConnected());
	return String(buffer);
}

//******************************************************************** 

pt::pintptr SocketI::getSocketHandle() throw (SocketException)
{
	check();
	return socket->get_handle();
}

//********************************************************************
//********************************************************************

Socket::Socket(const InetAddress& remote, int port, int timeout) throw (IOException) 
: StrongReference<SocketI>(new SocketI(remote, port, timeout))
{}

//********************************************************************

Socket::Socket(const InetAddress& remote, 
			   int remoteport, 
			   const InetAddress& local, 
			   int localport) throw (IOException)
			   : StrongReference<SocketI>(new SocketI(remote, remoteport, local, localport))
{}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
