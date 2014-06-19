// ZipUtility.h

//********************************************************************

#ifndef jlj_util_zip_ZipUtility_h
#define jlj_util_zip_ZipUtility_h

//********************************************************************

#include <jlj/lang/Object.h>
using namespace jlj::lang;
#include <jlj/io/Exceptions.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(zip)

//********************************************************************

class ExportedByJLJutil ZipUtility 
{
public:
	~ZipUtility(); // final
	ZipUtility();

public:
	static const int NO_COMPRESSION;
	static const int BEST_SPEED;
	static const int BEST_COMPRESSION;
	static const int DEFAULT_COMPRESSION;

	/**
	* you should "binary or" this with a level to give a hint to the corresponding inputstream
	* that processing the inputstream should happen in an extra thread because it takes a long time
	* and would otherwise block other i/o
	*/
	static const int USE_THREAD;								

public:
	/**
	*
	* if out points to a valid buffer (eg. the in buffer)
	* out will be deleted and initialized with a new buffer containing the result
	*
	* the caller must use delete [] out to free the out buffer
	*
	*/
	void zipBuffer(
		char*& out,
		int& outsize,
		const char* in, 
		int insize, 
		int compressionlevel = BEST_SPEED) throw (IOException);

	void unzipBuffer(
		char*& out,
		int& outsize,
		const char* in, 
		int insize) throw (IOException);

	static char* zipCookieNoCompression();
	static char* zipCookieWithCompression();
	static char* zipCookieNoCompressionThreaded();
	static char* zipCookieWithCompressionThreaded();

};

//********************************************************************

NAMESPACE_END(zip)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
