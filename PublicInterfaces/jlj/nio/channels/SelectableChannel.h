// SelectableChannel.h

//********************************************************************

#ifndef jlj_nio_SelectableChannel_h
#define jlj_nio_SelectableChannel_h

//********************************************************************

#include <jlj/nio/channels/Selector.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

struct ExportedByJLJnio SelectableChannel; 
class ExportedByJLJnio SelectionKeyI; 

//********************************************************************

class ExportedByJLJnio SelectableChannelI 
	: public ObjectI
{
protected:
	virtual ~SelectableChannelI() = 0;
	friend class TypedReference<SelectableChannelI>;
	SelectableChannelI(); 

public:
	virtual void close() throw (IOException);
	virtual bool isOpen() const = 0;
	virtual SelectableChannel configureBlocking(bool); // currently only supports "true"
	virtual bool isBlocking() const; // currently will only return true
	virtual bool isRegistered() const;
	virtual SelectionKey keyFor(Selector) const;
	virtual SelectionKey registerWith(Selector, int ops) throw (ClosedSelectorException, CancelledKeyException);
	virtual SelectionKey registerWith(Selector, int ops, Object attachment) throw (ClosedSelectorException, CancelledKeyException);
	virtual int validOps() = 0;

private:
	bool blocking;
	bool registered;
	WeakReference<SelectionKeyI> selectionkey;

};

//********************************************************************

StrongReferenceCommonDeclaration(SelectableChannel, ExportedByJLJnio);

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
