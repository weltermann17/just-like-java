// ChunkedOutputStream.h

//********************************************************************

#ifndef jlj_util_ChunkedOutputStream
#define jlj_util_ChunkedOutputStream

//********************************************************************

#include <jlj/io/AbstractOutputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

struct ExportedByJLJutil ChunkBufferI
{
	~ChunkBufferI();
	ChunkBufferI();

	void clear();

	int capacity;
	int position;
	int limit;
	char* buffer;

	static const int DEFAULTCAPACITY;
	static const int ENDOFBUFFER;
	static const int ENDOFFILE;

};

//********************************************************************

typedef  ExportedByJLJutil ChunkBufferI* ChunkBuffer;

//********************************************************************

class ExportedByJLJutil ChunkedOutputStreamI
	: public AbstractOutputStreamI
{
protected:
	~ChunkedOutputStreamI() throw (IOException);
	friend class TypedReference<ChunkedOutputStreamI>;

public:
	ChunkedOutputStreamI(OutputStream) throw (IOException);

public:
	void close() throw (IOException);
	void flush() throw (IOException);
	void write(const char*, int offset, int length) throw (IOException);
	int tell() const throw (IOException);

private:
	OutputStream outputstream;
	ChunkBuffer chunk;
	int total;
	bool closed;

};

//********************************************************************
//********************************************************************

struct ExportedByJLJio ChunkedOutputStream : StrongReference<ChunkedOutputStreamI>
{
	StrongReferenceCommonDerivateImplementation(ChunkedOutputStream);
};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
