// CRC.h

//********************************************************************

#ifndef jlj_io_CRC_h
#define jlj_io_CRC_h

//********************************************************************

#include <jlj/lang/UTF8String.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio CRC 
{
public:
	~CRC(); // final
	CRC();

public:	

	/**
	*
	* @example
	*
	* int c = 0;
	* while (!feof(f)) {
	*	// read part from file into mybuffer
	*	c = computeCRC(mybuffer, mybuffersize, c);
	* }
	*
	*/
	unsigned int CRC::compute(const char* buffer, size_t len, unsigned int& crc);

private:
	static const unsigned int crctable[];

};

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
