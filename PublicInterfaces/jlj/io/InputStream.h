// InputStream.h

//********************************************************************

#ifndef jlj_io_InputStream_h
#define jlj_io_InputStream_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
#include <jlj/lang/UTF8String.h>
using namespace jlj::lang;

//********************************************************************

#include <jlj/io/Exceptions.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio InputStreamI
	: public ObjectI
{
public:
	~InputStreamI() = 0;
	InputStreamI();

public:
	virtual void close() throw (IOException) = 0;

	virtual int available() const throw (IOException) = 0;
	virtual void mark(int) throw (IOException) = 0;
	virtual bool markSupported() const = 0;
	virtual void reset() throw (IOException) = 0;
	virtual int skip(int) throw (IOException) = 0;

	virtual char read() throw (IOException) = 0;
	virtual int read(char*, int length) throw (IOException) = 0;
	virtual int read(char*, int offset, int length) throw (IOException) = 0;

	// some add-ons to the Java API

	virtual int read(UTF8String&, int expectedsize = 0) throw (IOException) = 0;

	// reads until length is reached or returns 0 (stream closed) or -1 (at EOF)
	virtual int readComplete(char*, int offset, int length) throw (IOException) = 0;

	virtual int tell() const throw (IOException) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(InputStream, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
