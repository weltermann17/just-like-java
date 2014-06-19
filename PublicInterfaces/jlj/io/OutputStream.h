// OutputStream.h

//********************************************************************

#ifndef jlj_io_OutputStream_h
#define jlj_io_OutputStream_h

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

class ExportedByJLJio OutputStreamI
	: public ObjectI
{
public:
	~OutputStreamI() = 0;
	OutputStreamI();

public:
	virtual void close() throw (IOException) = 0;
	virtual void flush() throw (IOException) = 0;

	virtual void write(const char*, int offset, int length) throw (IOException) = 0;
	virtual void write(const char*, int length) throw (IOException) = 0;
	virtual void write(char) throw (IOException) = 0;

	virtual int tell() const throw (IOException) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(OutputStream, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
