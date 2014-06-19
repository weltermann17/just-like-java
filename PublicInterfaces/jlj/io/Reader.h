// Reader.h

//********************************************************************

#ifndef jlj_io_Reader_h
#define jlj_io_Reader_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;

//********************************************************************

#include <jlj/io/Exceptions.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio ReaderI
	: public ObjectI
{
public:
	~ReaderI() = 0;
	ReaderI();

public:
	virtual void close() throw (IOException) = 0;

	virtual void mark(int readaheadlimit) throw (IOException) = 0;
	virtual bool markSupported() const = 0;

	virtual void reset() throw (IOException) = 0;
	virtual bool ready() const throw (IOException) = 0;
	virtual int skip(int) throw (IOException) = 0;

	virtual wchar_t read() throw (IOException) = 0;
	virtual int read(String&, int expectedsize = 0) throw (IOException) = 0;
	virtual int read(wchar_t*, int length) throw (IOException) = 0;
	virtual int read(wchar_t*, int offset, int length) throw (IOException) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Reader, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
