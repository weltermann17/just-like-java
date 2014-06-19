// MemoryMapping.h

//********************************************************************

#ifndef jlj_io_helper_MemoryMapping_h
#define jlj_io_helper_MemoryMapping_h

//********************************************************************

#include <jlj/lang/Object.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)
NAMESPACE_BEGIN(helper)

//********************************************************************

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

//********************************************************************

#ifdef _MSC_VER
#define STAT struct _stat
#define FILENO _fileno
#define FSTAT _fstat
#include <io.h>
#else
#define STAT struct stat
#define FILENO fileno
#define FSTAT fstat
#endif

//********************************************************************

class ExportedByJLJio MemoryMapping 
{
public:
	/**
	* returns 0 if mapping fails else a well-mapped pointer
	*/
	static char* map(
		char* buffer, 
		int filesize, 
		FILE* file, 
		int offset);

	/**
	* returns false if unmapping fails
	*/
	static bool unmap(
		char* buffer,
		int filesize);

private:
	static int getPageSize();

	static const int PROT_READ;
	static const int MAP_PRIVATE;
	static char* MAP_FAILED;

	~MemoryMapping();	
	MemoryMapping();

};

//********************************************************************

NAMESPACE_END(helper)
NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
