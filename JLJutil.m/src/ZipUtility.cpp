// ZipUtility.cpp

//*******************************************************************

#include <jlj/util/zip/ZipUtility.h>
#include "zlib.h"

//*******************************************************************

#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

#include <string.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(zip)

//*******************************************************************

#define CHECK_ERR(err) if (Z_OK != (err)) { delete [] buf; throw IOException(L"ZipUtility failed"); }
#define CHECK_EOF(err) if (Z_STREAM_END != (err) && Z_OK != (err)) { delete [] buf; throw IOException(L"ZipUtility : EOF error"); }

//*******************************************************************

const int ZipUtility::NO_COMPRESSION = 1;
const int ZipUtility::BEST_SPEED = 2;
const int ZipUtility::BEST_COMPRESSION = 4;
const int ZipUtility::DEFAULT_COMPRESSION = 8;

const int ZipUtility::USE_THREAD = 16;

//*******************************************************************

char COOKIE_NOCOMPRESSION[] = { 'Z', 'I', 'P' , '0' , 'Z', 'I', 'P' , '0', 0 };
char COOKIE_WITHCOMPRESSION[] = { 'Z', 'I', 'P' , '1' , 'Z', 'I', 'P' , '0', 0 };
char COOKIE_NOCOMPRESSION_THREADED[] = { 'Z', 'I', 'P' , '0' , 'Z', 'I', 'P' , '1', 0 };
char COOKIE_WITHCOMPRESSION_THREADED[] = { 'Z', 'I', 'P' , '1' , 'Z', 'I', 'P' , '1', 0 };

const int COOKIESIZE = 8;

//*******************************************************************
//*******************************************************************

ZipUtility::~ZipUtility()
{}

//*******************************************************************

ZipUtility::ZipUtility()
{}

//*******************************************************************

void ZipUtility::zipBuffer(char*& out,
						   int& outsize,
						   const char* in, 
						   int insize, 
						   int compressionlevel) throw (IOException)
{	
	// the out buffer can be bigger than the in buffer
	outsize = (insize + 128) * 1.1; 

	char* buf = new char[outsize];

	z_stream c_stream;

	c_stream.zalloc = (alloc_func) 0;  
	c_stream.zfree = (free_func) 0;
	c_stream.opaque = (voidpf) 0;

	int ziplevel;
	switch (compressionlevel & ~USE_THREAD)
	{
	case NO_COMPRESSION:
		ziplevel = 0;
		break;
	case BEST_SPEED:
		ziplevel = 1;
		break;
	case BEST_COMPRESSION:
		ziplevel = 9;
		break;
	case DEFAULT_COMPRESSION:
	default:
		ziplevel = -1;
		break;
	}
	char* COOKIE;
	switch (compressionlevel & USE_THREAD)
	{
	case USE_THREAD:
		if (0 == ziplevel) COOKIE = COOKIE_NOCOMPRESSION_THREADED; else COOKIE = COOKIE_WITHCOMPRESSION_THREADED;
		break;
	default:
		if (0 == ziplevel) COOKIE = COOKIE_NOCOMPRESSION; else COOKIE = COOKIE_WITHCOMPRESSION;
		break;
	}

	CHECK_ERR(deflateInit(&c_stream, ziplevel));

	c_stream.next_in = (unsigned char*) in; 
	c_stream.avail_in = insize;

	c_stream.next_out = (unsigned char*) buf;
	c_stream.avail_out = outsize;

	CHECK_EOF(deflate(&c_stream, Z_FINISH));
	CHECK_ERR(deflateEnd(&c_stream));

#ifdef WIN32
	insize = bigendian(insize);
#endif
	outsize = c_stream.total_out;
	memcpy(buf + outsize, &insize, sizeof(int)); 

	outsize += sizeof(int);
	memcpy(buf + outsize, COOKIE, COOKIESIZE);

	outsize += COOKIESIZE;

	if (0 != out) delete [] out;
	out = buf;
}

//*******************************************************************

void ZipUtility::unzipBuffer(char*& out,
							 int& outsize,
							 const char* in, 
							 int insize) throw (IOException)
{		
	insize -= COOKIESIZE;
	if (0 > insize) return;
	
	char cookie[COOKIESIZE];
	memcpy(cookie, in + insize, COOKIESIZE);
	if (0 != memcmp(cookie, COOKIE_NOCOMPRESSION, COOKIESIZE) &&
		0 != memcmp(cookie, COOKIE_WITHCOMPRESSION, COOKIESIZE) &&
		0 != memcmp(cookie, COOKIE_NOCOMPRESSION_THREADED, COOKIESIZE) &&
		0 != memcmp(cookie, COOKIE_WITHCOMPRESSION_THREADED, COOKIESIZE))
	{
		return;
	}
	insize -= sizeof(int);
	if (0 > insize) return;
	memcpy((char*) &outsize, in + insize, sizeof(int)); 
#ifdef WIN32
	outsize = bigendian(outsize);
#endif

	char* buf = new char[outsize];

	z_stream d_stream; 

	d_stream.zalloc = (alloc_func) 0;  
	d_stream.zfree = (free_func) 0;
	d_stream.opaque = (voidpf) 0;

	CHECK_ERR(inflateInit(&d_stream));

	d_stream.next_in  = (unsigned char*) in;
	d_stream.avail_in = insize;

	d_stream.next_out = (unsigned char*) buf;
	d_stream.avail_out = outsize;

	CHECK_EOF(inflate(&d_stream, Z_FINISH));
	CHECK_ERR(inflateEnd(&d_stream));

	if (0 != out) delete [] out;
	out = buf;
}

//********************************************************************

char* ZipUtility::zipCookieNoCompression()
{
	return COOKIE_NOCOMPRESSION;
}

//********************************************************************

char* ZipUtility::zipCookieWithCompression()
{
	return COOKIE_WITHCOMPRESSION;
}

//********************************************************************

char* ZipUtility::zipCookieNoCompressionThreaded()
{
	return COOKIE_NOCOMPRESSION_THREADED;
}

//********************************************************************

char* ZipUtility::zipCookieWithCompressionThreaded()
{
	return COOKIE_WITHCOMPRESSION_THREADED;
}

//********************************************************************

NAMESPACE_END(zip)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//*******************************************************************

// eof
