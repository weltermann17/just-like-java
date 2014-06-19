// AbstractInputStream.h

//********************************************************************

#ifndef jlj_io_AbstractInputStream_h
#define jlj_io_AbstractInputStream_h

//********************************************************************

#include <jlj/io/InputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio AbstractInputStreamI
	: public InputStreamI
{
public:
	~AbstractInputStreamI() throw (IOException);
	AbstractInputStreamI();

public:
	int available() const throw (IOException);
	void close() throw (IOException);
	void mark(int readAheadLimit) throw (IOException);
	bool markSupported() const;
	void reset() throw (IOException);
	int skip(int) throw (IOException);
	int tell() const throw (IOException);

	char read() throw (IOException);
	int read(char*, int length) throw (IOException);
	int read(char*, int offset, int length) throw (IOException) = 0;

	int read(UTF8String&, int expectedsize = 0) throw (IOException);
	int readComplete(char*, int offset, int length) throw (IOException);

};

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
