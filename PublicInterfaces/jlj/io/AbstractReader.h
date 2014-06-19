// AbstractReader.h

//********************************************************************

#ifndef jlj_io_AbstractReader_h
#define jlj_io_AbstractReader_h

//********************************************************************

#include <jlj/io/Reader.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio AbstractReaderI
	: public ReaderI
{
public:
	~AbstractReaderI() throw (IOException);
	AbstractReaderI();

public:
	void close() throw (IOException);
	void mark(int readAheadLimit) throw (IOException);
	bool markSupported() const;
	void reset() throw (IOException);
	bool ready() const throw (IOException);
	int skip(int) throw (IOException);
	wchar_t read() throw (IOException);
	int read(wchar_t*, int length) throw (IOException);
	int read(wchar_t*, int offset, int length) throw (IOException) = 0;

};

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
