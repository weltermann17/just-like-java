// ZipOutputStream.h

//********************************************************************

#ifndef ZipOutputStream_h
#define ZipOutputStream_h

//********************************************************************

#include <jlj/util/zip/ZipUtility.h>
#include <jlj/io/AbstractOutputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(zip)

//********************************************************************

class ExportedByJLJutil ZipOutputStreamI
	: public AbstractOutputStreamI
{
public:
	~ZipOutputStreamI() throw (IOException);
	ZipOutputStreamI(OutputStream, int compression = ZipUtility::BEST_SPEED) throw (IOException);

public:
	void close() throw (IOException);
	void flush() throw (IOException);
	void write(const char*, int offset, int length) throw (IOException);
	int tell() const throw (IOException);

private:
	void flushBuffer() throw (IOException);

	OutputStream outputstream;
	ZipUtility zip;
	int compression;
	bool usecompression;
	char* buffer;
	int total;
	int position;
	int limit;
	int capacity;

	static const int DEFAULTCAPACITY;

};

//********************************************************************

StrongReferenceCommonDeclaration(ZipOutputStream, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(zip)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
