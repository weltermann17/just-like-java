// FileReader.cpp

//********************************************************************

#ifndef FileReader_cpp
#define FileReader_cpp

//********************************************************************

#include <jlj/io/FileReader.h>
#include <jlj/io/helper/MemoryMapping.h>
#include <jlj/core/ptypes/pasync.h>

//********************************************************************

#include <stdio.h>
#include <wchar.h>

//********************************************************************

#if !defined(_WINDOWS_SOURCE)
#include <iconv.h>
#endif

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

FileReaderI::~FileReaderI() throw (IOException)
{
	close();
}

//********************************************************************

FileReaderI::FileReaderI(const String& name) throw (IOException) 
: FileHelperI(name, "rt")
{
	buffersize = bytesnotread = FileHelperI::filesize();
	inptr = buffer = MemoryMapping::map(0, buffersize, file, 0);
	if (0 == buffer) throw IOException(WITHDETAILS(L"MemoryMapping::map() failed."));
}

//********************************************************************

FileReaderI::FileReaderI(FILE* file) throw (IOException)
: FileHelperI(file)
{
	buffersize = bytesnotread = FileHelperI::filesize();
	inptr = buffer = MemoryMapping::map(0, buffersize, file, 0);
	if (0 == buffer) throw IOException(WITHDETAILS(L"MemoryMapping::map() failed."));
}

//********************************************************************

void FileReaderI::close() throw (IOException)
{
	// make sure to unmap first before closing the file

	MemoryMapping::unmap(buffer, buffersize);
	FileHelperI::close();
	buffer = inptr = 0; buffersize = bytesnotread = 0;
}

//********************************************************************

bool FileReaderI::ready() const throw (IOException)
{
	return 0 < bytesnotread;
}

//********************************************************************

void FileReaderI::reset() throw (IOException)
{
	MemoryMapping::unmap(buffer, buffersize);
	buffersize = bytesnotread = FileHelperI::filesize();
	inptr = buffer = MemoryMapping::map(0, buffersize, file, 0);
	if (0 == buffer) throw IOException(WITHDETAILS(L"MemoryMapping::map() failed."));
}

//********************************************************************

int FileReaderI::skip(int bytestoskip) throw (IOException)
{
#if defined(_WINDOWS_SOURCE) 
	int newposition = (FileHelperI::filesize() - bytesnotread) + bytestoskip;

	if(newposition < 0) throw IOException(L"FileReaderI::skip() failed"); 
	if(newposition > FileHelperI::filesize()) throw IOException(L"FileReaderI::skip() failed"); 

	reset();

	const int BUFSIZE = 8*1024;
	wchar_t buf[BUFSIZE];
	int n = 0;
	int total = 0;
	while (total < newposition)
	{
		n = read(buf, 0, BUFSIZE < (newposition - total) ? BUFSIZE : (newposition - total));
		total += n;
	}	
#else
	if (bytestoskip > bytesnotread)
	{
		bytestoskip = bytesnotread;
	}
	inptr += bytestoskip;
	if(inptr < buffer) throw IOException(L"FileReaderI::skip() failed"); 
	if(inptr > buffer + FileHelperI::filesize()) throw IOException(L"FileReaderI::skip() failed"); 

	bytesnotread -= bytestoskip;
#endif
	
	return bytestoskip;
}

//********************************************************************

int FileReaderI::read(String& result, int expectedsize) throw (IOException)
{
	const int BUFSIZE = 8*1024;
	wchar_t buf[BUFSIZE];
	int n = 0;
	int total = 0;
	
	StringBuffer buffer;
	if (0 < expectedsize) buffer->ensureCapacity(expectedsize);

	while (0 < (n = read(buf, 0, BUFSIZE)))
	{
		total += n;
		buffer->append(buf, 0, n);
	}
	result = String(buffer); // consumes buffer	
	return total;
}

//********************************************************************

int FileReaderI::read(wchar_t* result, int offset, int length) throw (IOException)
{
#if defined(_WINDOWS_SOURCE) 
	
	const int VIEWSIZE = 64*1024;

	int	destlen = MultiByteToWideChar(
		CP_UTF8,
		0,
		inptr,
		bytesnotread,
		result + offset,
		length);

	if (0 == destlen)
	{
		if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError())
		{
			destlen = length;
		}
	}
	else
	{
		destlen = destlen;
	}

	int sourcelen = WideCharToMultiByte(
		CP_UTF8,
		0,
		result + offset,
		destlen,
		0,
		0,
		0,
		0);

	inptr += sourcelen;
	bytesnotread -= sourcelen;

	if (VIEWSIZE < inptr - buffer)
	{
		MemoryMapping::unmap(buffer, buffersize);
		buffersize = bytesnotread;
		inptr = buffer = MemoryMapping::map(0, buffersize, file, inptr - buffer);
		if (0 == buffer) throw IOException(WITHDETAILS(L"MemoryMapping::map() failed."));
	}

	validate();
	return destlen;

#else

	static iconv_t cd = 0;
	if (0 == cd)
	{
#if defined(PLATEFORME_DS64)
		// degrades performance when called for every read
		cd = iconv_open("UTF-32", "UTF-8");
#else
		cd = iconv_open("UTF-16", "UTF-8");
#endif
	}

	if (0 == bytesnotread) return -1;

	char* outptr = (char*) (result + offset);
	size_t outlen = length * sizeof(wchar_t);
	size_t destlen = outlen;

	static pt::rwlock* rw = new pt::rwlock();
	pt::scopewrite scope(*rw);

	size_t rc = iconv(
		cd,
		&inptr,
		&bytesnotread,
		&outptr,
		&outlen);

	// never called: iconv_close()

	validate();
	return (destlen - outlen) / sizeof(wchar_t);

#endif
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
