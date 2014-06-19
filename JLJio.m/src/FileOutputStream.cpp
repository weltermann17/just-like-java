// FileOutputStream.cpp

//********************************************************************

#ifndef FileOutputStream_cpp
#define FileOutputStream_cpp

//********************************************************************

#include <jlj/io/FileOutputStream.h>
#include <jlj/lang/UTF8String.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

FileOutputStreamI::~FileOutputStreamI() throw (IOException)
{
	close();
}

//********************************************************************

FileOutputStreamI::FileOutputStreamI(const String& name) throw (IOException)
: FileHelperI(name, "wb")
{}

//********************************************************************

FileOutputStreamI::FileOutputStreamI(const String& name, 
									 bool append, 
									 bool textmode) throw (IOException)
: FileHelperI(name, (append ? (textmode ?  "at" : "ab" ) : (textmode ? "wt" : "wb")))
{}

//********************************************************************

FileOutputStreamI::FileOutputStreamI(FILE* file) throw (IOException)
: FileHelperI(file) 
{}

//********************************************************************

void FileOutputStreamI::close() throw (IOException)
{
	FileHelperI::close();
}

//********************************************************************

void FileOutputStreamI::flush() throw (IOException)
{
	FileHelperI::flush();
}

//********************************************************************

void FileOutputStreamI::write(const char* value, int offset, int length) throw (IOException)
{
	fwrite(value + offset, 1, length, file);
	validate();
}

//********************************************************************

int FileOutputStreamI::tell() const throw (IOException)
{
	validate();
	return ftell(file);
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
