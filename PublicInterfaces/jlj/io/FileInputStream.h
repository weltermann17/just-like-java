// FileInputStream.h

//********************************************************************

#ifndef jlj_io_FileInputStream_h
#define jlj_io_FileInputStream_h

//********************************************************************

#include <jlj/io/AbstractInputStream.h>
#include <jlj/io/helper/FileHelper.h>
using namespace jlj::io::helper;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio FileInputStreamI
	: public AbstractInputStreamI
	, protected FileHelperI
{
protected:
	~FileInputStreamI() throw (IOException);
	friend class TypedReference<FileInputStreamI>;

public:
	FileInputStreamI(const String&) throw (IOException);
	FileInputStreamI(FILE*) throw (IOException);

public:
	void close() throw (IOException);

	int available() const throw (IOException);
	void mark(int) throw (IOException);
	bool markSupported() const;
	void reset() throw (IOException);
	int skip(int) throw (IOException);

	int read(char*, int offset, int length) throw (IOException);
	int read(UTF8String&, int) throw (IOException);
	int tell() const throw (IOException);

private:
	int bookmark;
	char* buffer;
	size_t buffersize; 
	char* inptr;
	size_t bytesnotread;

};

//********************************************************************

StrongReferenceCommonDeclaration(FileInputStream, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
