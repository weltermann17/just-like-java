// FileInputStream.cpp

//********************************************************************

#ifndef FileInputStream_cpp
#define FileInputStream_cpp

//********************************************************************

#include <jlj/io/FileInputStream.h>
#include <jlj/io/helper/MemoryMapping.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

FileInputStreamI::~FileInputStreamI() throw (IOException)
{
	close();
}

//********************************************************************

FileInputStreamI::FileInputStreamI(const String& name) throw (IOException) 
: FileHelperI(name, "rb")
, bookmark(0)
{
#if !defined(_WINDOWS_SOURCE) 
	buffersize = bytesnotread = FileHelperI::filesize();
	inptr = buffer = MemoryMapping::map(0, buffersize, file, 0);
	if (0 == buffer) throw IOException(WITHDETAILS(L"MemoryMapping::map() failed."));
#endif
}

//********************************************************************

FileInputStreamI::FileInputStreamI(FILE* file) throw (IOException)
: FileHelperI(file)
, bookmark(0)
{
#if !defined(_WINDOWS_SOURCE) 
	buffersize = bytesnotread = FileHelperI::filesize();
	inptr = buffer = MemoryMapping::map(0, buffersize, file, 0);
	if (0 == buffer) throw IOException(WITHDETAILS(L"MemoryMapping::map() failed."));
#endif
}

//********************************************************************

void FileInputStreamI::close() throw (IOException)
{
#if !defined(_WINDOWS_SOURCE) 
	MemoryMapping::unmap(buffer, buffersize);
	buffer = inptr = 0; buffersize = bytesnotread = 0;
#endif
	FileHelperI::close();
}

//********************************************************************

int FileInputStreamI::available() const throw (IOException)
{
	validate();
#if defined(_WINDOWS_SOURCE) 
	if (feof(file))
	{
		return -1;
	}
	else
	{
		return filesize() - ftell(file);
	}
#else
	return bytesnotread;
#endif
}

//********************************************************************

void FileInputStreamI::mark(int) throw (IOException)
{
#if defined(_WINDOWS_SOURCE) 
	bookmark = ftell(file);
#else
	bookmark = inptr - buffer;
#endif
	validate();
}

//********************************************************************

bool FileInputStreamI::markSupported() const
{
	return true;
}

//********************************************************************

void FileInputStreamI::reset() throw (IOException)
{
#if defined(_WINDOWS_SOURCE) 
	fseek(file, bookmark, SEEK_SET);
#else
	inptr = buffer + bookmark; 
	bytesnotread = buffersize - bookmark;
#endif
	validate();
}

//********************************************************************

int FileInputStreamI::skip(int n) throw (IOException)
{
#if defined(_WINDOWS_SOURCE) 
	int last = ftell(file);
	validate();
	if (0 == fseek(file, n, SEEK_CUR))
	{
		validate();
		int result = ftell(file) - last;
		validate();
		return result;
	}
	else
		return 0;
#else
	if (buffer > inptr + n || buffer + buffersize < inptr + n) throw IOException(L"FileInputStreamI::skip() : Invalid value.");
	inptr += n;
	bytesnotread -= n;
	return n;
#endif
}

//********************************************************************

int FileInputStreamI::read(char* result, int offset, int length) throw (IOException)
{
	int n;
#if defined(_WINDOWS_SOURCE) 
	if (feof(file)) return -1;
	n = fread(result + offset, 1, length, file);
#else
	if (0 >= bytesnotread) return -1;
	n = length < bytesnotread ? length : bytesnotread;
	memcpy(result + offset, inptr, n);
	bytesnotread -= n;
	inptr += n;
#endif
	validate();
	return n;
}

//********************************************************************

int FileInputStreamI::read(UTF8String& result, int) throw (IOException)
{
	int n = filesize();
	char* buffer = new char[n + 1]; 
	buffer[n] = 0;
	n = readComplete(buffer, 0, n);
	result = UTF8String(buffer);
	delete [] buffer;
	return n;
}

//********************************************************************

int FileInputStreamI::tell() const throw (IOException)
{
#if defined(_WINDOWS_SOURCE) 
	int result = ftell(file);
	validate();
	return result;
#else
	return inptr - buffer; // not vice versa, I'm so stupid
#endif
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
