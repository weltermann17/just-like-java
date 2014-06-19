// FileOutputStream.h

//********************************************************************

#ifndef jlj_io_FileOutputStream_h
#define jlj_io_FileOutputStream_h

//********************************************************************

#include <jlj/io/AbstractOutputStream.h>
#include <jlj/io/helper/FileHelper.h>
using namespace jlj::io::helper;

//********************************************************************

#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio FileOutputStreamI 
	: public AbstractOutputStreamI
	, protected FileHelperI
{
protected:
	~FileOutputStreamI() throw (IOException);
	friend class TypedReference<FileOutputStreamI>;

public:
	FileOutputStreamI(const String&) throw (IOException);
	FileOutputStreamI(const String&, bool append, bool textmode = false) throw (IOException);
	FileOutputStreamI(FILE*) throw (IOException);

public:
	void close() throw (IOException);
	void flush() throw (IOException);

	void write(const char*, int offset, int length) throw (IOException);

	int tell() const throw (IOException);

};

//********************************************************************

StrongReferenceCommonDeclaration(FileOutputStream, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
