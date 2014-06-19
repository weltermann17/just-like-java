// InMemoryInputStream.h

//********************************************************************

#ifndef jlj_io_InMemoryInputStream_h
#define jlj_io_InMemoryInputStream_h

//********************************************************************

#include <jlj/io/AbstractInputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio InMemoryInputStreamI
	: public AbstractInputStreamI
{
protected:
	~InMemoryInputStreamI() throw (IOException);
	friend class TypedReference<InMemoryInputStreamI>;

public:
	InMemoryInputStreamI(const char* buffer, int buffersize) throw (IOException);

public:
	int available() const throw (IOException);
	void mark(int) throw (IOException);
	bool markSupported() const;
	void reset() throw (IOException);
	int skip(int) throw (IOException);

	int read(char*, int offset, int length) throw (IOException);

	int tell() const throw (IOException);

private:
	const char* buffer;
	int buffersize;
	int position;
	int bookmark;

};

//********************************************************************

StrongReferenceCommonDeclaration(InMemoryInputStream, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
