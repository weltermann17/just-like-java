// Demultiplexer.h

//********************************************************************

#ifndef jlj_nio_reactor_Demultiplexer_h
#define jlj_nio_reactor_Demultiplexer_h

//********************************************************************

#include <jlj/nio/reactor/ChunkedInputStream.h>
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
using namespace jlj::util::concurrent::atomic;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJLJnio DemultiplexerI 
	: public RunnableI
{
protected:
	~DemultiplexerI();
	friend class TypedReference<DemultiplexerI>;

public:
	DemultiplexerI(const String& id, ChunkedInputStream);

private:
	void run();

	String id;
	ChunkedInputStream chunkedinputstream;

};

//********************************************************************

StrongReferenceCommonDeclaration(Demultiplexer, ExportedByJLJnio);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
