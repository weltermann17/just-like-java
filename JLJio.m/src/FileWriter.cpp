// FileWriter.cpp

//********************************************************************

#ifndef FileWriter_cpp
#define FileWriter_cpp

//********************************************************************

#include <jlj/io/FileWriter.h>
#include <jlj/lang/UTF8String.h>

//********************************************************************

#include <stdio.h>
#include <wchar.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

FileWriterI::~FileWriterI() throw (IOException)
{
	close();
}

//********************************************************************

FileWriterI::FileWriterI(const String& name) throw (IOException)
: FileHelperI(name, "wt")
{}

//********************************************************************

FileWriterI::FileWriterI(const String& name, bool append) throw (IOException)
: FileHelperI(name, (append ? "at" : "wt"))
{}

//********************************************************************

FileWriterI::FileWriterI(FILE* file) throw (IOException)
: FileHelperI(file)
{}

//********************************************************************

void FileWriterI::close() throw (IOException)
{
	FileHelperI::close();
}

//********************************************************************

void FileWriterI::flush() throw (IOException)
{
	FileHelperI::flush();
}

//********************************************************************

void FileWriterI::write(const wchar_t* value, int offset, int length) throw (IOException)
{
	UTF8String out(value + offset, length);
	size_t w = 0;
	size_t o = 0;
	size_t l = out->length();
	do 
	{
		w = fwrite(constcharptr(out) + o, sizeof(char), l, file);
		l -= w;
		o += w;
	}
	while (0 < l && 0 == ferror(file));
	validate();
}

//********************************************************************

void FileWriterI::write(const wchar_t* value) throw (IOException)
{
	UTF8String out(value, wcslen(value));
	fputs(constcharptr(out), file);
	validate();
}

//********************************************************************

void FileWriterI::write(const String& value) throw (IOException)
{
	fputs(constcharptr(value->toUTF8String()), file);
	validate();
}

//********************************************************************

void FileWriterI::write(const String& value, int offset, int length) throw (IOException)
{
	write(constwchar_tptr(value), offset, length);
}

//********************************************************************

void FileWriterI::write(const UTF8String& value) throw (IOException)
{
	fputs(constcharptr(value), file);
	validate();
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
