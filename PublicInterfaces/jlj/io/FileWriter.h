// FileWriter.h

//********************************************************************

#ifndef jlj_io_FileWriter_h
#define jlj_io_FileWriter_h

//********************************************************************

#include <jlj/io/AbstractWriter.h>
#include <jlj/io/helper/FileHelper.h>
using namespace jlj::io::helper;

//********************************************************************

#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio FileWriterI
	: public AbstractWriterI
	, protected FileHelperI
{
protected:
	~FileWriterI() throw (IOException);
	friend class TypedReference<FileWriterI>;

public:
	FileWriterI(const String&) throw (IOException);
	FileWriterI(const String&, bool append) throw (IOException);
	FileWriterI(FILE*) throw (IOException);

public:
	void close() throw (IOException);
	void flush() throw (IOException);

	void write(const wchar_t*, int offset, int length) throw (IOException);
	void write(const wchar_t*) throw (IOException);
	void write(const String&) throw (IOException);
	void write(const String&, int offset, int length) throw (IOException);

	virtual void write(const UTF8String&) throw (IOException);

};

//********************************************************************

StrongReferenceCommonDeclaration(FileWriter, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
