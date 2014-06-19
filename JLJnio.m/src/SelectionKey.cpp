// SelectionKey.cpp

//********************************************************************

#ifndef jlj_nio_channels_SelectionKey_cpp
#define jlj_nio_channels_SelectionKey_cpp

//********************************************************************

#include <jlj/nio/channels/SelectionKey.h>
#include <jlj/nio/channels/Selector.h>
#include <jlj/nio/channels/SelectableChannel.h>
#include <jlj/util/Uuid.h>
using namespace jlj::util;

#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

const int SelectionKey::OP_ACCEPT = 1;
const int SelectionKey::OP_CONNECT = 2;
const int SelectionKey::OP_READ = 4;
const int SelectionKey::OP_WRITE = 8;

//********************************************************************

SelectionKeyI::~SelectionKeyI() 
{}

//********************************************************************

SelectionKeyI::SelectionKeyI(Selector selector,
							 SelectableChannel channel,
							 int ops) throw (ClosedSelectorException, CancelledKeyException)
							 : cancelled(false)
							 , interestops(ops)
							 , readyops(0)
							 , selector(selector)
							 , channel(channel)
							 , id(Uuid::randomUuid()->toString())
{}

//********************************************************************

bool SelectionKeyI::equals(const Object& object) const
{
	SelectionKey b;
	if (object->instanceOf(b))
	{
		return id->equals(b->toString());
	}
	else
	{
		return false;
	}
}

//********************************************************************

int SelectionKeyI::hashCode() const
{
	return id->hashCode();
}

//********************************************************************

String SelectionKeyI::toString() const
{
	return id 
		+ (isValid() ? L" valid " : L" invalid ") 
		+ (getChannel()->isOpen() ? L"open " : L"closed ") ;

}

//********************************************************************

Object SelectionKeyI::attach(const Object& object)
{
	Object old = attachedobject;
	attachedobject = object;
	return old;
}

//********************************************************************

Object SelectionKeyI::attachment() const
{
	return attachedobject;
}

//********************************************************************

void SelectionKeyI::cancel()
{
	cancelled->set(true);
}

//********************************************************************

SelectableChannel SelectionKeyI::getChannel() const
{
	return channel;
}

//********************************************************************

int SelectionKeyI::interestOps() const throw (CancelledKeyException)
{
	if (cancelled) throw CancelledKeyException(WITHDETAILS(L""));
	return interestops;
}

//********************************************************************

SelectionKey SelectionKeyI::interestOps(int ops) throw (IllegalArgumentException, CancelledKeyException)
{
	if (cancelled) throw CancelledKeyException(WITHDETAILS(L""));
	if (!isValid(ops)) throw IllegalArgumentException(WITHDETAILS(L""));
	this->interestops = ops;
	return this;
}

//********************************************************************

bool SelectionKeyI::isAcceptable() const throw (CancelledKeyException)
{
	if (cancelled) throw CancelledKeyException(WITHDETAILS(L""));
	return 0 != (readyops & SelectionKey::OP_ACCEPT);
}

//********************************************************************

bool SelectionKeyI::isConnectable() const throw (CancelledKeyException)
{
	if (cancelled) throw CancelledKeyException(WITHDETAILS(L""));
	return 0 != (readyops & SelectionKey::OP_CONNECT);
}

//********************************************************************

bool SelectionKeyI::isReadable() const throw (CancelledKeyException)
{
	if (cancelled) throw CancelledKeyException(WITHDETAILS(L""));
	return 0 != (readyops & SelectionKey::OP_READ);
}

//********************************************************************

bool SelectionKeyI::isWritable() const throw (CancelledKeyException)
{
	if (cancelled) throw CancelledKeyException(WITHDETAILS(L""));
	return 0 != (readyops & SelectionKey::OP_WRITE);
}

//********************************************************************

bool SelectionKeyI::isValid() const
{
	return !cancelled && isValid(interestops);
}

//********************************************************************

bool SelectionKeyI::isValid(int ops) const
{
	Selector s = selector;
	if (!s) return false;
	bool isnull = 0 == ops;
	bool validvalue = 0 != (ops & (SelectionKey::OP_ACCEPT | SelectionKey::OP_CONNECT | SelectionKey::OP_READ | SelectionKey::OP_WRITE));
	bool isopen = s->isOpen();
	return (isnull || validvalue) && isopen;
}

//********************************************************************

int SelectionKeyI::readyOps() const throw (CancelledKeyException)
{
	if (cancelled) throw CancelledKeyException(WITHDETAILS(L""));
	return readyops;
}

//********************************************************************

void SelectionKeyI::readyOps(int ops, bool bitwiseOr) 
{
	if (bitwiseOr) readyops = (readyops | ops); else readyops = ops;
}

//********************************************************************

Selector SelectionKeyI::getSelector() const
{
	return selector;
}

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
