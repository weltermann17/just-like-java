// ZipInputStream.h

//********************************************************************

#ifndef ZipInputStream_h
#define ZipInputStream_h

//********************************************************************

#include <jlj/util/zip/ZipUtility.h>
#include <jlj/io/AbstractInputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(zip)

//********************************************************************

class ExportedByJLJutil ZipInputStreamI
	: public AbstractInputStreamI
{
public:
	~ZipInputStreamI() throw (IOException);
	ZipInputStreamI(InputStream) throw (IOException);

public:
	void close() throw (IOException);
	int available() const throw (IOException);
	int read(char*, int offset, int length) throw (IOException);
	int tell() const throw (IOException);

	bool useThread() const;

private:
	void fillBuffer(int length) throw (IOException);

	InputStream inputstream;
	ZipUtility zip;
	bool compressed;
	bool usethread;
	char* buffer;
	int total;
	int position;
	int limit;
	int capacity;

	static const int DEFAULTCAPACITY;

};

//********************************************************************

StrongReferenceCommonDeclaration(ZipInputStream, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(zip)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
