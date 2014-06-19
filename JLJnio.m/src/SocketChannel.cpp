// SocketChannel.cpp

//********************************************************************

#ifndef SocketChannel_cpp
#define SocketChannel_cpp

//********************************************************************

#include <jlj/nio/channels/SocketChannel.h>
#include <jlj/nio/channels/SelectionKey.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

SocketChannelI::~SocketChannelI() 
{}

//********************************************************************

SocketChannelI::SocketChannelI(Socket socket)
: socket_(socket)
{}

//********************************************************************

void SocketChannelI::close() throw (IOException)
{
	SelectableChannelI::close();
	socket_->close();
}

//********************************************************************

bool SocketChannelI::isOpen() const
{
	return !socket_->isClosed();
}

//********************************************************************

Socket SocketChannelI::socket()
{
	return socket_;
}

//********************************************************************

int SocketChannelI::validOps()
{
	return SelectionKey::OP_CONNECT || SelectionKey::OP_READ || SelectionKey::OP_WRITE;
}

//********************************************************************
//********************************************************************

SocketChannel SocketChannel::open(SocketAddress socketaddress,
								  int timeout) throw (IOException)
{
	Socket socket(socketaddress->getAddress(), socketaddress->getPort(), timeout);
	return new SocketChannelI(socket);
}

//********************************************************************
NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
