// ChunkedInputStream.h

//********************************************************************

#ifndef jlj_nio_reactor_ChunkedInputStream_h
#define jlj_nio_reactor_ChunkedInputStream_h

//********************************************************************

#include <jlj/nio/reactor/Reactor.h>
#include <jlj/io/AbstractInputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJLJnio ChunkedInputStreamI
	: public AbstractInputStreamI
{
protected:
	~ChunkedInputStreamI() throw (IOException);
	friend class TypedReference<ChunkedInputStreamI>;

public:	
	ChunkedInputStreamI(const String& id) throw (IOException);

public:
	void close() throw (IOException);
	int available() const throw (IOException);
	int skip(int) throw (IOException);
	int read(char*, int offset, int length) throw (IOException);
	int tell() const throw (IOException);

public:
	void pipe(ChunkBuffer);
	void demultiplex();
	bool isClosed();

private:
	ChunkBuffers readbuffers;
	ChunkBuffer chunk;
	AtomicInteger total;
	AtomicBoolean eof;
	AtomicBoolean closed;
	int timeout;
	String id;

};

//********************************************************************

StrongReferenceCommonDeclaration(ChunkedInputStream, ExportedByJLJnio);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
