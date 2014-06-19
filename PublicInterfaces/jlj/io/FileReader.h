// FileReader.h

//********************************************************************

#ifndef jlj_io_FileReader_h
#define jlj_io_FileReader_h

//********************************************************************

#include <jlj/io/AbstractReader.h>
#include <jlj/io/helper/FileHelper.h>
using namespace jlj::io::helper;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio FileReaderI
	: public AbstractReaderI
	, protected FileHelperI
{
protected:
	~FileReaderI() throw (IOException);
	friend class TypedReference<FileReaderI>;

public:
	FileReaderI(FILE*) throw (IOException);
	FileReaderI(const String&) throw (IOException);

public:
	void close() throw (IOException);

	int read(String&, int expectedsize) throw (IOException);
	int read(wchar_t*, int offset, int length) throw (IOException);

	int skip(int bytestoskip) throw (IOException);
	void reset() throw (IOException);
	bool ready() const throw (IOException);


private:
	char* buffer;
	size_t buffersize; 
	char* inptr;
	size_t bytesnotread;

};

//********************************************************************

StrongReferenceCommonDeclaration(FileReader, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
