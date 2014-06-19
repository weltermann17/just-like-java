// ConnectionLifetimeManager.h

//********************************************************************

#ifndef jlj_nio_reactor_ConnectionLifetimeManager_h
#define jlj_nio_reactor_ConnectionLifetimeManager_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;

#include <jlj/nio/reactor/Reactor.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/Multiplexer.h>
#include <jlj/nio/reactor/ChunkedOutputStream.h>
#include <jlj/nio/reactor/ChunkedInputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJLJnio ConnectionLifetimeManagerI 
	: public ObjectI
{
protected:
	~ConnectionLifetimeManagerI();
	friend class TypedReference<ConnectionLifetimeManagerI>;

public:
	ConnectionLifetimeManagerI(
		const String& id,
		SocketChannel,
		Reactor);

	bool equals(const Object&) const;
	int hashCode() const;
	String toString() const;

public:
	Reactor getReactor() const;
	ExecutorService<int> getExecutorService() const;
	Connection getConnection() const;
	SocketChannel getSocketChannel() const;
	Multiplexer getMultiplexer() const;
	ChunkBuffers getSendBuffers() const;

	ChunkedInputStream getChunkedInputStream(const String& streamid) const;

public:
	enum EVENTS
	{
		FIRST = 0,
		NUL,				// the null event just to trigger the state machine
		NCS,			    // null chunk sent
		NCR,				//            recv
		NAS,			    // null anything sent
		NAR,				//               recv
		IOS,				// IOException external send
		IOR,				//                      recv
		OIC,				// one ChunkedInputStream closed
		CCP,			    // connection closed by peer
		LAST
	};

	enum STATES 
	{
		INITIAL = 0,		// all states unset
		ICS = 1,			// internal closed send
		ICR = 2,			//                 recv
		ECS = 4,			// external closed send
		ECR = 8,			//                 recv
		ISR = 16,			// internal shutting down recv
		DIS = 32,			// disconnected completely
		FINAL = 64			// last, sentinel
	};

	bool testIfSet(int);
	void addEvent(EVENTS);	

	bool isFinalized(); 
	void cleanup();

	String getEventAsString(EVENTS) const;
	String getStatesAsString() const;

private:
	void runFiniteStateMachine();

	void markInputstreamsAsClosed();
	bool checkInputstreamsAreClosed() const;
	void closeSocketChannel();

private:
	SocketChannel socketchannel;
	Reactor reactor;
	Connection connection;
	Multiplexer multiplexer;
	ChunkBuffers sendbuffers;
	SynchronizedMap<String, ChunkedInputStream> chunkedinputstreams;
	AtomicInteger states;
	BlockingQueue<EVENTS> events;
	Logger logger;
	String id;

};

//********************************************************************

#define testIfStateSet(X) \
	ConnectionLifetimeManager::getInstance(id)->testIfSet(ConnectionLifetimeManagerI::##X)

//********************************************************************

#define testIfStateUnSet(X) \
	!(ConnectionLifetimeManager::getInstance(id)->testIfSet(ConnectionLifetimeManagerI::##X))

//********************************************************************

#define fireEvent(X) \
	ConnectionLifetimeManager::getInstance(id)->addEvent(ConnectionLifetimeManagerI::##X)

//********************************************************************

struct ExportedByJLJnio ConnectionLifetimeManager
	: StrongReference<ConnectionLifetimeManagerI>
{
public:
	StrongReferenceCommonDerivateImplementation(ConnectionLifetimeManager);

	static ConnectionLifetimeManager getInstance(const String& id) throw (IndexOutOfBoundsException);
};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
