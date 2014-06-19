// ChunkBufferPool.h

//********************************************************************

#ifndef jlj_nio_reactor_ChunkBufferPool_h
#define jlj_nio_reactor_ChunkBufferPool_h

//********************************************************************

#include <jlj/util/concurrent/BlockingQueue.h>
using namespace jlj::util::concurrent;
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
using namespace jlj::util::concurrent::atomic;
#include <jlj/util/AbstractNamedSingleton.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

struct ExportedByJLJnio ChunkBufferI
{
	int capacity;
	int position;
	int limit;
	char* buffer;
};

//********************************************************************

typedef ExportedByJLJnio ChunkBufferI* ChunkBuffer;

//********************************************************************

class ExportedByJLJnio ChunkBufferPoolI 
	: public ObjectI
{
public:
	~ChunkBufferPoolI();
	ChunkBufferPoolI(); 

	ChunkBuffer get() throw (InterruptedException);
	void makeIdle(ChunkBuffer&);
	void clearBuffer(ChunkBuffer);

	int idles() const;
	int inUse() const;

	static const int DEFAULTCAPACITY;

private:
	ChunkBuffer createBuffer(int capacity);
	void destroyBuffer(ChunkBuffer);

	BlockingQueue<ChunkBuffer> idle;
	AtomicInteger inuse;
	const int initialsize;
	const int maxsize;
	Logger logger;

};

//********************************************************************

struct ExportedByJLJnio ChunkBufferPool 
	: StrongReference<ChunkBufferPoolI>
	, AbstractNamedSingleton<ChunkBufferPool, ChunkBufferPoolI>
{
	StrongReferenceCommonDerivateImplementation(ChunkBufferPool);
};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
