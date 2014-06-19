// Acceptor.cpp

//********************************************************************

#ifndef jlj_nio_reactor_Acceptor_cpp
#define jlj_nio_reactor_Acceptor_cpp

//********************************************************************

#include <jlj/nio/reactor/Acceptor.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/channels/ServerSocketChannel.h>
using namespace jlj::nio::channels;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

AcceptorI::~AcceptorI()
{
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::FINEST, L"AcceptorI::~AcceptorI()");
}

//********************************************************************

AcceptorI::AcceptorI(int port,
					 int backlog,
					 const InetAddress& bindaddress) throw (IOException)
					 : port(port)
					 , backlog(backlog)
					 , bindaddress(bindaddress)
					 , timeout(-1)
{}

//********************************************************************

void AcceptorI::handleEvent(SelectionKey key) 
{
	try
	{
		ServerSocketChannel serversocketchannel;
		key->getChannel()->downcast(serversocketchannel);

		SocketChannel socketchannel = serversocketchannel->accept();

		Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::INFO, L"Connection accepted.");

		socketchannel->socket()->setSoTimeout(timeout);

		Connection::open(socketchannel, reactor);
	}
	catch (const IOException& e)
	{
		Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(
			Level::SEVERE, L"AcceptorI::handleEvent() : " + e->toString());
	}
}

//********************************************************************

void AcceptorI::registerWith(Reactor reactor) 
{
	this->reactor = reactor;

	ServerSocketChannel serversocketchannel = 
		ServerSocketChannel::open(port, backlog, bindaddress);

	serversocketchannel->registerWith(
		reactor->getSelector(), 
		SelectionKey::OP_ACCEPT,
		this);

	reactor->getSelector()->wakeup();
}

//********************************************************************

void AcceptorI::setTimeout(int milliseconds)
{
	this->timeout = timeout;
}

//********************************************************************
//********************************************************************

Acceptor Acceptor::open(int port,
						int backlog,
						const InetAddress& bindaddress) throw (IOException)
{
	return new AcceptorI(port, backlog, bindaddress);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
