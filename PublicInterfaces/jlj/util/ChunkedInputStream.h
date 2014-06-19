// ChunkedInputStream.h

//********************************************************************

#ifndef jlj_util_ChunkedInputStream
#define jlj_util_ChunkedInputStream

//********************************************************************

#include <jlj/util/ChunkedOutputStream.h>
#include <jlj/io/AbstractInputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil ChunkedInputStreamI
	: public AbstractInputStreamI
{
protected:
	~ChunkedInputStreamI() throw (IOException);
	friend class TypedReference<ChunkedInputStreamI>;

public:
	ChunkedInputStreamI(InputStream) throw (IOException);

public:
	void close() throw (IOException);
	int available() const throw (IOException);
	int skip(int) throw (IOException);
	int read(char*, int offset, int length) throw (IOException);
	int tell() const throw (IOException);

private:
	ChunkBuffer readBuffer() throw (IOException);

	void readFully(
		ChunkBuffer& dst,
		ChunkBuffer& src,
		int l,
		int& avail);

	InputStream inputstream;
	ChunkBuffer chunk;
	int total;
	bool eof;

	int unavailable;
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
};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
