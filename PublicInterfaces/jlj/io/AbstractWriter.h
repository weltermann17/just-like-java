// AbstractWriter.h

//********************************************************************

#ifndef jlj_io_AbstractWriter_h
#define jlj_io_AbstractWriter_h

//********************************************************************

#include <jlj/io/Writer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio AbstractWriterI
	: public WriterI
{
public:
	~AbstractWriterI() throw (IOException);
	AbstractWriterI();

public:
	void close() throw (IOException);
	void flush() throw (IOException);

	void write(const wchar_t*, int offset, int length) throw (IOException) = 0;
	void write(const wchar_t*) throw (IOException);
	void write(const String&) throw (IOException);
	void write(const String&, int offset, int length) throw (IOException);
	void write(wchar_t) throw (IOException);

};

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
