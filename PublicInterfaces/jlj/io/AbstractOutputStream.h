// AbstractOutputStream.h

//********************************************************************

#ifndef jlj_io_AbstractOutputStream_h
#define jlj_io_AbstractOutputStream_h

//********************************************************************

#include <jlj/io/OutputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio AbstractOutputStreamI
	: public OutputStreamI
{
public:
	~AbstractOutputStreamI() throw (IOException);
	AbstractOutputStreamI();

public:
	void close() throw (IOException);
	void flush() throw (IOException);

	void write(const char*, int offset, int length) throw (IOException) = 0;
	void write(const char*, int length) throw (IOException);
	void write(char) throw (IOException);

};

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
