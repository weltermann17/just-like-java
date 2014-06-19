// Multiplexer.h

//********************************************************************

#ifndef jlj_nio_reactor_Multiplexer_h
#define jlj_nio_reactor_Multiplexer_h

//********************************************************************

#include <jlj/nio/reactor/ChunkBufferPool.h>
#include <jlj/nio/reactor/Reactor.h>
#include <jlj/nio/channels/SocketChannel.h>
using namespace jlj::nio::channels;
#include <jlj/util/concurrent/BlockingQueue.h>
using namespace jlj::util::concurrent;
#include <jlj/util/concurrent/locks/Lock.h>
using namespace jlj::util::concurrent::locks;
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
using namespace jlj::util::concurrent::atomic;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

struct ExportedByJLJnio MultiplexedSocket;
class ExportedByJLJnio MultiplexedSocketI;
struct ExportedByJLJnio Connection;
class ExportedByJLJnio ConnectionI;

//********************************************************************

class ExportedByJLJnio MultiplexerI 
	: public ObjectI
{
protected:
	~MultiplexerI() throw (IOException);
	friend class TypedReference<MultiplexerI>;

public:
	MultiplexerI(const String& id);

	void parseBuffer(InputStream) throw (IOException);

private:
	void readFully(ChunkBuffer& dst, ChunkBuffer& src, int len, const String&, const String&, int& avail);

	int unavailable;
	String streamid;
	ChunkBuffer incomplete;
	ChunkBuffer destination;
	int state;
	int nextstate;

	enum { 
		STATE_INITIAL = 0,
		STATE_CONNECTION,
		STATE_COMPLETECONNECTION,
		STATE_COMMAND,
		STATE_COMPLETECOMMAND,
		STATE_INCOMPLETEREAD,
		STATE_COMPLETEREAD,
		STATE_ENDOFBUFFER,
		STATE_ENDOFFILE,
		STATE_BUFFER,
		STATE_INCOMPLETEBUFFERREAD,
		STATE_COMPLETEBUFFER,
		STATE_FINAL,
		STATE_INVALID = -1
	};

	String id;

};

//********************************************************************

struct ExportedByJLJnio Multiplexer : StrongReference<MultiplexerI> 
{    
	StrongReferenceCommonDerivateImplementation(Multiplexer);

	static const int IDENTIFIERLENGTH;
	static const int ENDOFFILE;
	static const int ENDOFBUFFER;

};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
