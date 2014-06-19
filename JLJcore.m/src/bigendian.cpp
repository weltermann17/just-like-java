// bigendian.cpp

//********************************************************************

#ifndef bigendian_cpp
#define bigendian_cpp

//********************************************************************

#include <jlj/core/platform/bigendian.h>
#include <jlj/core/debug/assert.h>

//********************************************************************

#include <memory.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(platform)

//********************************************************************

bool needToConvertBigEndian()
{
#if NEEDTOCONVERT
	return true;
#if 0
	static bool firsttime = true;	
	static bool needtoconvert;

	if (firsttime)
	{
		firsttime = false;
		const char* magiccookie_lo = "ANYT";
		const char* magiccookie_hi = "HING";
		int cookie_lo;
		int cookie_hi;
		memcpy((char*)&cookie_lo, magiccookie_lo, sizeof(int));
		memcpy((char*)&cookie_hi, magiccookie_hi, sizeof(int));

		int MAGICCOOKIE_LO = 0x414E5954;	// ANYT
		int MAGICCOOKIE_HI = 0x48494E47;	// HING

		needtoconvert = MAGICCOOKIE_LO == bigendian(cookie_lo) 
			&& MAGICCOOKIE_HI == bigendian(cookie_hi);
	}

	return needtoconvert;
#endif
#else
	return false;
#endif
}

//********************************************************************

short bigendian(short littleendian)
{
#if NEEDTOCONVERT
	struct twobyte
	{
		char lo;
		char hi;
	};
	twobyte little = *(twobyte*)&littleendian;
	char swap = little.lo;
	little.lo = little.hi;
	little.hi = swap;
	return *(short*)(&little);
#else
	return littleendian;
#endif
}

//********************************************************************

wchar_t bigendian(wchar_t littleendian)
{
#if NEEDTOCONVERT
	struct twobyte
	{
		char lo;
		char hi;
	};
	twobyte little = *(twobyte*)&littleendian;
	char swap = little.lo;
	little.lo = little.hi;
	little.hi = swap;
	return *(wchar_t*)(&little);
#else
	return littleendian;
#endif
}

//********************************************************************

int bigendian(int littleendian)
{
#if NEEDTOCONVERT
	struct twobyte
	{
		char lo;
		char hi;
	};
	struct fourbyte
	{
		twobyte lo;
		twobyte hi;
	};
	fourbyte little = *(fourbyte*)&littleendian;
	char swap = little.lo.lo;
	little.lo.lo = little.hi.hi;
	little.hi.hi = swap;
	swap = little.hi.lo;
	little.hi.lo = little.lo.hi;
	little.lo.hi = swap;
	return *(int*)(&little);
#else
	return littleendian;
#endif
}

//********************************************************************

long bigendian(long littleendian)
{
#if NEEDTOCONVERT
	ASSERT(4 == sizeof(long));
	struct twobyte
	{
		char lo;
		char hi;
	};
	struct fourbyte
	{
		twobyte lo;
		twobyte hi;
	};
	fourbyte little = *(fourbyte*)&littleendian;
	char swap = little.lo.lo;
	little.lo.lo = little.hi.hi;
	little.hi.hi = swap;
	swap = little.hi.lo;
	little.hi.lo = little.lo.hi;
	little.lo.hi = swap;
	return *(long*)(&little);
#else
	return littleendian;
#endif
}

//********************************************************************

double bigendian(double littleendian)
{
#if NEEDTOCONVERT
	struct twobyte
	{
		char lo;
		char hi;
	};
	struct fourbyte
	{
		twobyte lo;
		twobyte hi;
	};
	struct eightbyte
	{
		fourbyte lo;
		fourbyte hi;
	};
	eightbyte little = *(eightbyte*)&littleendian;
	char swap = little.lo.lo.lo;
	little.lo.lo.lo = little.hi.hi.hi;
	little.hi.hi.hi = swap;
	swap = little.lo.hi.lo;
	little.lo.hi.lo = little.hi.lo.hi;
	little.hi.lo.hi = swap;
	swap = little.hi.lo.lo;
	little.hi.lo.lo = little.lo.hi.hi;
	little.lo.hi.hi = swap;
	swap = little.hi.hi.lo;
	little.hi.hi.lo = little.lo.lo.hi;
	little.lo.lo.hi = swap;
	return *(double*)(&little);
#else
	return littleendian;
#endif
}

//********************************************************************
#ifdef _WINDOWS_SOURCE
#include <psapi.h>
#include <stdio.h>
#endif
//********************************************************************

const char* getCurrentProcessName()
{
#ifdef _WINDOWS_SOURCE
	static char buf[MAX_PATH];
	buf[0] = 0;
	char* basename = buf;
	DWORD basenamemaxlength = sizeof(buf);

	int n = 0;
	if (0 < (n = GetModuleBaseNameA(GetCurrentProcess(), 0, basename, basenamemaxlength)))
	{
		buf[n] = 0;
	}
	else
	{
		fprintf(stdout, "jlj::core::platform::getCurrentProcessName() : GetModuleBaseName(GetCurrentProcessId()) failed.\n");
	}

	return basename;
#else
	fprintf(stdout, "jlj::core::platform::getCurrentProcessName() : Not implemented on Unix platforms.\n");
#endif
}

//********************************************************************

NAMESPACE_END(platform)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
