// Writer.h

//********************************************************************

#ifndef jlj_io_Writer_h
#define jlj_io_Writer_h

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

class ExportedByJLJio WriterI
	: public ObjectI
{
public:
	virtual ~WriterI() = 0;
	WriterI();

public:
	virtual void close() throw (IOException) = 0;
	virtual void flush() throw (IOException) = 0;

	virtual void write(const wchar_t*, int offset, int length) throw (IOException) = 0;
	virtual void write(const wchar_t*) throw (IOException) = 0;
	virtual void write(const String&) throw (IOException) = 0;
	virtual void write(const String&, int offset, int length) throw (IOException) = 0;
	virtual void write(wchar_t) throw (IOException) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Writer, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
