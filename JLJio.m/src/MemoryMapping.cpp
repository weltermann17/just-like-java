// MemoryMapping.cpp

//********************************************************************

#ifndef MemoryMapping_cpp
#define MemoryMapping_cpp

//********************************************************************

#include <jlj/io/helper/MemoryMapping.h>

//********************************************************************

#ifdef _MSC_VER
#include <winbase.h>
#include <io.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#include <iconv.h>
#endif

//********************************************************************

#ifdef _MSC_VER
static SECURITY_ATTRIBUTES MMAP_SECURITY_ATTRIBUTES = { sizeof(SECURITY_ATTRIBUTES), 0, TRUE };
#endif

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)
NAMESPACE_BEGIN(helper)

//********************************************************************

MemoryMapping::~MemoryMapping() 
{}

//********************************************************************

MemoryMapping::MemoryMapping()
{}

//********************************************************************

char* MemoryMapping::map(char* buffer, 
						 int buffersize, 
						 FILE* file, 
						 int offset) 
{
#ifdef _MSC_VER

	HANDLE filehandle = (HANDLE) _get_osfhandle(FILENO(file));

	HANDLE filemapping = ::CreateFileMappingA(
		filehandle, 
		&MMAP_SECURITY_ATTRIBUTES,
		PAGE_READONLY | SEC_COMMIT, 
		0, 
		0,
		0);

	if (0 == filemapping) return MAP_FAILED;

	DWORD alignedoffset = (offset / getPageSize()) * getPageSize();
	DWORD alignment = offset - alignedoffset;

	LPVOID mappedmemory = ::MapViewOfFile(
		filemapping,
		FILE_MAP_READ,
		0, 
		alignedoffset, 
		0);

	::CloseHandle(filemapping);

	if (0 != mappedmemory)
	{
		return ((char*) mappedmemory) + alignment;
	}
	else	
	{
		return 0;
	}

#else

	int filehandle = FILENO(file);

	int protection = PROT_READ;
	int flags = MAP_PRIVATE;

	int alignedoffset = (offset / getpagesize()) * getpagesize();
	int alignment = offset - alignedoffset;
	int alignedlength = ((buffersize / getpagesize()) + 1) * getpagesize();

	char* result = (char*) ::mmap(
		(void*) buffer, 
		alignedlength, 
		protection,
		flags, 
		filehandle, 
		alignedoffset);

	return ((char*) -1) == result ? 0 : result + alignment;

#endif
}

//********************************************************************

bool MemoryMapping::unmap(char* buffer,
						  int buffersize) 
{
	if (0 != buffer)
	{
#ifdef _MSC_VER

		return ::UnmapViewOfFile(buffer) ? true : false;

#else

		int alignedlength = ((buffersize / getpagesize()) + 1) * getpagesize();
		return 0 == ::munmap((void*) buffer, alignedlength) ? true : false;

#endif
	}
	else
	{
		return 0;
	}
}

//********************************************************************

int MemoryMapping::getPageSize()
{
	static int pagesize = 0;	

	if (0 == pagesize)
	{
#ifdef WIN32
		SYSTEM_INFO si;
		::GetSystemInfo(&si);
		pagesize = si.dwAllocationGranularity;
#else
		pagesize = getpagesize();
#endif
	}   
	return pagesize;
}

//********************************************************************

#undef PROT_READ
#undef MAP_PRIVATE
#undef MAP_FAILED

const int MemoryMapping::PROT_READ = 1;
const int MemoryMapping::MAP_PRIVATE = 2;
char* MemoryMapping::MAP_FAILED = ((char*) -1);


//********************************************************************

NAMESPACE_END(helper)
NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
