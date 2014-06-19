// ServerSocketChannel.cpp

//********************************************************************

#ifndef ServerSocketChannel_cpp
#define ServerSocketChannel_cpp

//********************************************************************

#include <jlj/nio/channels/ServerSocketChannel.h>
#include <jlj/nio/channels/SelectionKey.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

ServerSocketChannelI::~ServerSocketChannelI() 
{}

//********************************************************************

ServerSocketChannelI::ServerSocketChannelI(ServerSocket socket)
: socket_(socket)
{}

//********************************************************************

SocketChannel ServerSocketChannelI::accept() throw (IOException)
{
	return new SocketChannelI(socket_->accept());
}

//********************************************************************

void ServerSocketChannelI::close() throw (IOException)
{
	SelectableChannelI::close();
	socket_->close();
}

//********************************************************************

bool ServerSocketChannelI::isOpen() const
{
	return !socket_->isClosed();
}

//********************************************************************

ServerSocket ServerSocketChannelI::socket()
{
	return socket_;
}

//********************************************************************

int ServerSocketChannelI::validOps()
{
	return SelectionKey::OP_ACCEPT; 
}

//********************************************************************
//********************************************************************

ServerSocketChannel ServerSocketChannel::open(int port, 
											  int backlog, 
											  const InetAddress& bindaddress) throw (IOException)
{
	ServerSocket socket(port, backlog, bindaddress);
	return new ServerSocketChannelI(socket);
}

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
