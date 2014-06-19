// ChunkedOutputStream.h

//********************************************************************

#ifndef jlj_nio_reactor_ChunkedOutputStream_h
#define jlj_nio_reactor_ChunkedOutputStream_h

//********************************************************************

#include <jlj/nio/reactor/Reactor.h>
#include <jlj/io/AbstractOutputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJLJnio ChunkedOutputStreamI
	: public AbstractOutputStreamI
{
protected:
	~ChunkedOutputStreamI() throw (IOException);
	friend class TypedReference<ChunkedOutputStreamI>;

public:	
	ChunkedOutputStreamI(
		const String& id,
		const String& streamid) throw (IOException);

public:
	void close() throw (IOException);
	void flush() throw (IOException);
	void write(const char*, int offset, int length) throw (IOException);
	int tell() const throw (IOException);

private:
	String id;
	char* ids;
	ChunkBuffer chunk;
	int total;
	bool closed;

};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
