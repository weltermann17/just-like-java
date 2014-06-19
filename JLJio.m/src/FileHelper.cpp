// FileHelper.cpp

//********************************************************************

#ifndef FileHelper_cpp
#define FileHelper_cpp

//********************************************************************

#include <jlj/io/helper/FileHelper.h>
#include <jlj/io/helper/MemoryMapping.h>
#include <jlj/lang/UTF8StringBuffer.h>

//********************************************************************

#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)
NAMESPACE_BEGIN(helper)

//********************************************************************

FileHelperI::~FileHelperI() throw (IOException)
{
	close();
}

//********************************************************************

FileHelperI::FileHelperI(const String& name, 
						 const char* mode) throw (IOException)
						 : filename(name->toUTF8String())
						 , ownfilehandle(true)
						 , length(0)
{
	file = fopen(constcharptr(filename), mode);
	validate();
}

//********************************************************************

FileHelperI::FileHelperI(FILE* file) throw (IOException)
: file(file)
#ifdef WIN32
, filename(UTF8StringBuffer()->append((pt::large) file))
#else
, filename((long) file)
#endif
, ownfilehandle(false)
, length(0)
{
	validate();
}

//********************************************************************

void FileHelperI::close() throw (IOException)
{
	if (ownfilehandle)
	{
		fclose(file);
		validate();
		file = 0;
		ownfilehandle = false;
	}
}

//********************************************************************

void FileHelperI::flush() throw (IOException)
{
	fflush(file);
	validate();
}

//********************************************************************

void FileHelperI::validate() const throw (IOException)
{
	char* s = 0;
	if (0 == file) 
	{
		s = strerror(errno);
	}
	else
	{
		int e = ferror(file);
		if (0 != e) s = strerror(e);
	}
	if (0 != s)
	{
		UTF8StringBuffer msg;
		msg->append("Operation failed: ");
		msg->append(filename);
		msg->append(" (");
		msg->append(s);
		msg->append(")");
		throw IOException(msg->toString());
	}
}

//********************************************************************

int FileHelperI::filesize() const throw (IOException)
{
	if (0 == length)
	{
		STAT info;
		int filehandle = FILENO(file);
		validate();

		if (0 == FSTAT(filehandle, &info))
		{
			length = (int) info.st_size;
		}
	}
	validate();
	return length;
}

//********************************************************************

NAMESPACE_END(helper)
NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
