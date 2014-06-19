// ZipInputStream.cpp

//********************************************************************

#ifndef ZipInputStream_cpp
#define ZipInputStream_cpp

//********************************************************************

#include <jlj/util/zip/ZipInputStream.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(zip)

//********************************************************************

const int ZipInputStreamI::DEFAULTCAPACITY = 128*1024;

//********************************************************************

ZipInputStreamI::~ZipInputStreamI() throw (IOException)
{
	close();
	delete [] buffer;
}

//********************************************************************

ZipInputStreamI::ZipInputStreamI(InputStream inputstream) throw (IOException)
: inputstream(inputstream) 
, buffer(new char[DEFAULTCAPACITY])
, total(0)
, position(0)
, limit(0)
, capacity(DEFAULTCAPACITY)
{
	int l = strlen(ZipUtility::zipCookieNoCompression());
	position = inputstream->readComplete(buffer, 0, l);

	compressed = 
		0 == strncmp(buffer, ZipUtility::zipCookieWithCompression(), l) ||
		0 == strncmp(buffer, ZipUtility::zipCookieWithCompressionThreaded(), l);

	usethread = 
		0 == strncmp(buffer, ZipUtility::zipCookieNoCompressionThreaded(), l) ||
		0 == strncmp(buffer, ZipUtility::zipCookieWithCompressionThreaded(), l);
}

//********************************************************************

void ZipInputStreamI::close() throw (IOException)
{
}

//********************************************************************

int ZipInputStreamI::available() const throw (IOException)
{
	return compressed ? limit : inputstream->available();
}

//********************************************************************

int ZipInputStreamI::read(char* result, int offset, int length) throw (IOException)
{
	if (compressed)
	{
		if (length <= limit)
		{
			memcpy(result + offset, buffer + position, length);

			total += length;
			position += length;
			limit -= length;

			return length;
		}
		else
		{
			if (0 < limit)
			{
				memcpy(result + offset, buffer + position, limit);

				int readsofar = limit;

				offset += limit;
				length -= limit;

				total += limit;
				position += limit;
				limit -= limit;

				fillBuffer(length);
				return readsofar + read(result, offset, length); // recursive
			}
			else if (0 == limit)
			{
				fillBuffer(length);
				return read(result, offset, length);
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		int n = inputstream->read(result, offset, length);
		total += n;
		return n;
	}
}

//********************************************************************

int ZipInputStreamI::tell() const throw (IOException)
{
	return total;
}

//********************************************************************

bool ZipInputStreamI::useThread() const 
{
	return usethread;
}

//********************************************************************

void ZipInputStreamI::fillBuffer(int length) throw (IOException) 
{
	position = 0;

	limit = 0;

	inputstream->readComplete((char*) &limit, 0, sizeof(int));
	limit = bigendian(limit);

	if (0 == limit)
	{
		limit = -1;
		return;
	} 
	else if (limit > capacity)
	{
		capacity = limit;
		char* newbuffer = new char[capacity];
		delete [] buffer;
		buffer = newbuffer;
	}

	inputstream->readComplete(buffer, 0, limit);

	if (compressed)
	{
		zip.unzipBuffer(
			buffer,
			limit,
			buffer, 
			limit);
	}

	capacity = limit;
}

//********************************************************************

NAMESPACE_END(zip)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
