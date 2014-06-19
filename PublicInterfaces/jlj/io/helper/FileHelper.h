// FileHelper.h

//********************************************************************

#ifndef jlj_io_helper_FileHelper_h
#define jlj_io_helper_FileHelper_h

//********************************************************************

#include <jlj/lang/UTF8String.h>
using namespace jlj::lang;
#include <jlj/io/Exceptions.h>
#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)
NAMESPACE_BEGIN(helper)

//********************************************************************

class ExportedByJLJio FileHelperI
{
protected:
	~FileHelperI() throw (IOException);

	FileHelperI(const String&, const char* mode) throw (IOException);
	FileHelperI(FILE*) throw (IOException);

	void close() throw (IOException);
	void flush() throw (IOException);

	void validate() const throw (IOException);
	int filesize() const throw (IOException);

	FILE* file;
	UTF8String filename;

private:
	bool ownfilehandle;
	mutable int length;

};

//********************************************************************

NAMESPACE_END(helper)
NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
