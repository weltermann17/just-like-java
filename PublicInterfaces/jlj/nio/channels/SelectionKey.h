// SelectionKey.h

//********************************************************************

#ifndef jlj_nio_SelectionKey_h
#define jlj_nio_SelectionKey_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Throwable.h>
using namespace jlj::lang;
#include <jlj/io/Exceptions.h>
using namespace jlj::io;
#include <jlj/nio/channels/Selector.h>
#include <jlj/nio/channels/SelectableChannel.h>
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
using namespace jlj::util::concurrent::atomic;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

struct ExportedByJLJnio SelectionKey; 

//********************************************************************

class ExportedByJLJnio SelectionKeyI 
	: public ObjectI
{

protected:
	~SelectionKeyI();
	friend class TypedReference<SelectionKeyI>;

	SelectionKeyI(Selector, SelectableChannel, int ops) throw (ClosedSelectorException, CancelledKeyException); 
	friend class ExportedByJLJnio SelectableChannelI; 
	
public:
	bool equals(const Object&) const;
	int hashCode() const;
	String toString() const;
	Object attach(const Object&);
	Object attachment() const;
	void cancel();
	SelectableChannel getChannel() const;
	int interestOps() const throw (CancelledKeyException);
	SelectionKey interestOps(int) throw (IllegalArgumentException, CancelledKeyException);
	bool isAcceptable() const throw (CancelledKeyException);
	bool isConnectable() const throw (CancelledKeyException);
	bool isReadable() const throw (CancelledKeyException);
	bool isValid() const;
	bool isWritable() const throw (CancelledKeyException);
	int readyOps() const throw (CancelledKeyException);
	Selector getSelector() const;

private:
	bool isValid(int ops) const;
	void readyOps(int ops, bool bitwiseOr = false);
	friend class ExportedByJLJnio SelectorI;

private:
	WeakReference<SelectorI> selector;
	SelectableChannel channel;
	Object attachedobject;
	AtomicInteger interestops;
	AtomicInteger readyops;
	AtomicBoolean cancelled;
	const String id;

};

//********************************************************************

struct ExportedByJLJnio SelectionKey : StrongReference<SelectionKeyI> 
{    
	StrongReferenceCommonDerivateImplementation(SelectionKey);

	static const int OP_ACCEPT;
	static const int OP_CONNECT;
	static const int OP_READ;
	static const int OP_WRITE;
};

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
