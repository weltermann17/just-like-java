// Connector.cpp

//********************************************************************

#ifndef Connector_cpp
#define Connector_cpp

//********************************************************************

#include <jlj/nio/reactor/Connector.h>
#include <jlj/nio/channels/SocketChannel.h>
using namespace jlj::nio::channels;
#include <jlj/util/StringAnything.h>
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

ConnectorI::~ConnectorI()
{}

//********************************************************************

ConnectorI::ConnectorI(Reactor reactor, 
					   int timeout)
					   : reactor(reactor)
					   , timeout(timeout)
{}

//********************************************************************

Connection ConnectorI::connect(SocketAddress socketaddress) throw (IOException)
{
	if (0 < reactor->shutdowntimeout)
	{
		throw IOException(WITHDETAILS(L"Reactor is already shutting down"));
	}

	SocketChannel socketchannel = SocketChannel::open(socketaddress, timeout);
	socketchannel->socket()->setSoTimeout(timeout);
	return Connection::open(socketchannel, reactor);	
}

//********************************************************************

Connection ConnectorI::connect(const String& connectionstring) throw (IOException)
{
	if (0 < reactor->shutdowntimeout)
	{
		throw IOException(WITHDETAILS(L"Reactor is already shutting down"));
	}

	StringBuffer buf(connectionstring);
	String hostname = buf->substring(0, buf->indexOf(L":"));
	Anything port = new StringAnythingI(buf->indexOf(L":"));
	return connect(new InetSocketAddressI(hostname, port));
}

//********************************************************************

void ConnectorI::setTimeout(int milliseconds)
{
	this->timeout = timeout;
}

//********************************************************************

void ConnectorI::registerWith(Reactor)
{
	// only necessary with async connecting
}

//********************************************************************

void ConnectorI::handleEvent(SelectionKey)
{
	// only necessary with async connecting
}

//********************************************************************
//********************************************************************

Connector Connector::open(Reactor reactor, 
						  int timeout)
{
	return new ConnectorI(reactor, timeout);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
