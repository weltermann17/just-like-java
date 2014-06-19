// ZipOutputStream.cpp

//********************************************************************

#ifndef ZipOutputStream_cpp
#define ZipOutputStream_cpp

//********************************************************************

#include <jlj/util/zip/ZipOutputStream.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(zip)

//********************************************************************

const int ZipOutputStreamI::DEFAULTCAPACITY = 1*1024*1024;

//********************************************************************

ZipOutputStreamI::~ZipOutputStreamI() throw (IOException) 
{
	close(); 
	delete [] buffer;
}

//********************************************************************

ZipOutputStreamI::ZipOutputStreamI(OutputStream outputstream, int compression) throw (IOException)
: outputstream(outputstream) 
, compression(compression)
, usecompression(ZipUtility::NO_COMPRESSION != (compression & ~ZipUtility::USE_THREAD))
, buffer(new char[DEFAULTCAPACITY])
, total(0)
, position(0)
, limit(DEFAULTCAPACITY)
, capacity(DEFAULTCAPACITY)
{
	bool usethread =  0 != (ZipUtility::USE_THREAD & compression);
	compression &= ~ZipUtility::USE_THREAD;

	if (compression != ZipUtility::DEFAULT_COMPRESSION && 
		compression != ZipUtility::BEST_COMPRESSION &&
		compression != ZipUtility::BEST_SPEED &&
		compression != ZipUtility::NO_COMPRESSION)
	{
		throw IOException(WITHDETAILS(L"Invalid compression level."));
	}

	char* cookie;
	if (usecompression) 
	{
		if (usethread) 
		{
			cookie = ZipUtility::zipCookieWithCompressionThreaded(); 
		}
		else 
		{
			cookie = ZipUtility::zipCookieWithCompression();
		}
	}
	else if (usethread) 
	{
		cookie = ZipUtility::zipCookieNoCompressionThreaded(); 
	}
	else 
	{
		cookie = ZipUtility::zipCookieNoCompression();
	}

	outputstream->write(cookie, strlen(cookie));
}

//********************************************************************

void ZipOutputStreamI::close() throw (IOException) 
{
	flush();

	// do NOT close the underlying outputstream here!
}

//********************************************************************

void ZipOutputStreamI::flush() throw (IOException) 
{
	flushBuffer();

	// do NOT flush the underlying outputstream here!
}

//********************************************************************

void ZipOutputStreamI::flushBuffer() throw (IOException) 
{
	if (0 < position)
	{
		char* out = 0; 
		int outsize = 0;

		zip.zipBuffer(
			out,
			outsize,
			buffer, 
			position,
			compression);

		if (0 < outsize) 
		{
			int o = bigendian(outsize);
			outputstream->write((char*) &o, 0, sizeof(int));
			outputstream->write(out, 0, outsize);
		}

		delete [] out;

		position = 0;
		limit = capacity;
	}
}

//********************************************************************

void ZipOutputStreamI::write(const char* value, int offset, int length) throw (IOException) 
{
	if (usecompression)
	{
		if (length <= limit)
		{
			memcpy(buffer + position, value + offset, length);

			total += length;		
			position += length;
			limit -= length;
		}
		else
		{
			memcpy(buffer + position, value + offset, limit);

			offset += limit;
			length -= limit;

			total += limit;
			position += limit;
			limit -= limit;

			flushBuffer();

			write(value, offset, length); // recursive
		}
	}
	else
	{
		outputstream->write(value, offset, length);
		total += length;
	}
}

//********************************************************************

int ZipOutputStreamI::tell() const throw (IOException)
{
	return total;
}

//********************************************************************

NAMESPACE_END(zip)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
