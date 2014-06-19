// Reactor.h

//********************************************************************

#ifndef jlj_nio_reactor_Reactor_h
#define jlj_nio_reactor_Reactor_h

//********************************************************************

#include <jlj/nio/reactor/ChunkBufferPool.h>
#include <jlj/lang/Runnable.h>
#include <jlj/nio/channels/Selector.h>
using namespace jlj::nio::channels;
#include <jlj/nio/reactor/Handler.h>
#include <jlj/util/Collections.h>
#include <jlj/util/Anything.h>
using namespace jlj::util;
#include <jlj/util/concurrent/BlockingQueue.h>
#include <jlj/util/concurrent/ExecutorService.h>
using namespace jlj::util::concurrent;
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

typedef ExportedByJLJnio BlockingQueue<Anything> AnythingQueue;
typedef BlockingQueue<ChunkBuffer> ChunkBuffers;

//********************************************************************

class ExportedByJLJnio ConnectionI;
struct ExportedByJLJnio Connection;
class ExportedByJLJnio ConnectionLifetimeManagerI;
struct ExportedByJLJnio ConnectionLifetimeManager;

//********************************************************************

class ExportedByJLJnio ReactorI 
	: public RunnableI
	, private CallableI<int>
{
public:
	~ReactorI() throw (IOException);
	ReactorI(ExecutorService<int>, int timeout) throw (IOException); 

	void run();

	virtual Selector getSelector() const;

	virtual AnythingQueue receiveQueue();
	
	virtual Connection getConnectionByRemoteIdentifier(const String&) const;

	virtual int getTimeout() const;
	virtual void setTimeout(int milliseconds);

	virtual void initializeShutdown(int timeout);
	virtual bool isShutdown() const;

	virtual void setShutdownOnLastConnectionClosed(bool);

	virtual void setPropagateIOExceptions(bool);
	virtual bool getPropagateIOExceptions() const;

protected:
	friend struct ExportedByJLJnio Connection;
	friend class ExportedByJLJnio ConnectionLifetimeManagerI;

	virtual void addConnectionId(const String& id);
	virtual ExecutorService<int> getExecutorService() const;
	virtual void startWriting();
	virtual void stopWriting();
	virtual ChunkBuffers getSendBuffers();
	
private:
	int call() throw (jlj::lang::Exception);
	void cleanup();
	friend class ExportedByJLJnio ConnectionI;
	friend class ExportedByJLJnio ConnectorI;

private:
	ExecutorService<int> executorservice;
	Selector selector;
	AnythingQueue receivequeue;
	ChunkBuffers sendbuffers; 
	BlockingQueue<int> writingstopped;
	int cleanupinterval;
	Logger logger;
	AtomicInteger timeout;
	AtomicInteger shutdowntimeout;
	AtomicBoolean isshutdown;
	AtomicBoolean shutdownonlastconnectionclosed;
	AtomicBoolean propagateioexceptions;
	SynchronizedList<String> connectionids;

};

//********************************************************************

struct ExportedByJLJnio Reactor : StrongReference<ReactorI> 
{    
	StrongReferenceCommonDerivateImplementation(Reactor);

	static Reactor open(ExecutorService<int>, int timeout = -1) throw (IOException);
	static const String REACTORS_LOGGER_NAME;
};

//********************************************************************
//********************************************************************

ExportedByJLJnio void initializeReactors() throw (Exception);

//********************************************************************

ExportedByJLJnio void finalizeReactors() throw (Exception);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
