// Connection.h

//********************************************************************

#ifndef jlj_nio_reactor_Connection_h
#define jlj_nio_reactor_Connection_h

//********************************************************************

#include <jlj/nio/reactor/Reactor.h>
#include <jlj/nio/reactor/Multiplexer.h>
#include <jlj/nio/reactor/Demultiplexer.h>
#include <jlj/nio/channels/SocketChannel.h>
using namespace jlj::nio::channels;
#include <jlj/util/Anything.h>
using namespace jlj::util;
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
using namespace jlj::util::concurrent::atomic;
#include <jlj/util/zip/ZipUtility.h> 
using namespace jlj::util::zip;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJLJnio ConnectionI 
	: public HandlerI
{
protected:
	~ConnectionI();
	friend class TypedReference<ConnectionI>;

public:
	ConnectionI(const String& id); 

	void handleEvent(SelectionKey);
	void registerWith(Reactor);

	virtual String getLocalIdentifier() const throw (IOException);
	virtual String getRemoteIdentifier() const throw (IOException);

	virtual void initializeShutdown() throw (IOException);

	virtual void send(Anything, bool jsonencoding) throw (IOException); // no compression
	virtual void send(Anything, bool jsonencoding, int compressionlevel) throw (IOException);

public:
	static const String NULLSTREAMID;

private:
	friend class ExportedByJLJnio ReactorI;
	friend class ExportedByJLJnio DemultiplexerI;

	mutable String localidentifier;
	mutable String remoteidentifier;

	String id;

};

//********************************************************************

struct ExportedByJLJnio Connection : StrongReference<ConnectionI> 
{    
	StrongReferenceCommonDerivateImplementation(Connection);

	static Connection open(SocketChannel, Reactor);
};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
