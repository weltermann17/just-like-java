// UTF8StringInputStream.h

//********************************************************************

#ifndef jlj_io_UTF8StringInputStream_h
#define jlj_io_UTF8StringInputStream_h

//********************************************************************

#include <jlj/io/AbstractInputStream.h>

#include <jlj/lang/UTF8String.h>
using jlj::lang::UTF8String;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio UTF8StringInputStreamI
	: public AbstractInputStreamI
{
protected:
	~UTF8StringInputStreamI() throw (IOException);
	friend class TypedReference<UTF8StringInputStreamI>;

public:
	UTF8StringInputStreamI(const UTF8String&);
	UTF8StringInputStreamI(UTF8String&); // swap buffers

public:
	int available() const throw (IOException);
	void mark(int) throw (IOException);
	bool markSupported() const;
	void reset() throw (IOException);
	int skip(int) throw (IOException);

	int read(char*, int offset, int length) throw (IOException);

	int tell() const throw (IOException);

private:
	UTF8String buffer;
	const char* bufferp;
	int buffersize;
	int position;
	int bookmark;

};

//********************************************************************

StrongReferenceCommonDeclaration(UTF8StringInputStream, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
