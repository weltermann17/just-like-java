// SelectableChannel.cpp

//********************************************************************

#ifndef SelectableChannel_cpp
#define SelectableChannel_cpp

//********************************************************************

#include <jlj/nio/channels/SelectableChannel.h>
#include <jlj/nio/channels/SelectionKey.h>

#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

SelectableChannelI::~SelectableChannelI() 
{}

//********************************************************************

SelectableChannelI::SelectableChannelI()
: blocking(true)
, registered(false)
{}

//********************************************************************

void SelectableChannelI::close() throw (IOException)
{
	SelectionKey key = selectionkey;
	if (!!key) key->getSelector()->unregisterFrom(key);
}

//********************************************************************

SelectableChannel SelectableChannelI::configureBlocking(bool) 
{
	blocking = true;
	return this;
}

//********************************************************************

bool SelectableChannelI::isBlocking() const
{
	return blocking;
}

//********************************************************************

bool SelectableChannelI::isRegistered() const
{
	return registered;
}

//********************************************************************

SelectionKey SelectableChannelI::keyFor(Selector selector) const
{
	if (registered)
	{
		return selectionkey;
	}
	else
	{
		SelectionKey empty;
		return empty;
	}
}

//********************************************************************

SelectionKey SelectableChannelI::registerWith(Selector selector, int ops) throw (ClosedSelectorException, CancelledKeyException) 
{
	Object dummy;
	return registerWith(selector, ops, dummy);
}

//********************************************************************

SelectionKey SelectableChannelI::registerWith(Selector selector, int ops, Object attachment) throw (ClosedSelectorException, CancelledKeyException) 
{
	if (!selector->isOpen()) 
	{
		throw ClosedSelectorException(WITHDETAILS(L""));
	}

	if (registered)
	{
		SelectionKey key = selectionkey;
		if (!!key && !key->isValid()) throw CancelledKeyException(WITHDETAILS(L""));
	}
	else
	{
		SelectionKey key = new SelectionKeyI(selector, this, ops);
		key->attach(attachment);
		selector->registerWith(key);
		selectionkey = key;
		registered = true;
	}
	return selectionkey;
}

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
