/*
 *
 *  C++ Portable Types Library (PTypes)
 *  Version 2.1.1  Released 27-Jun-2007, patched by Guido Schmidt, 20090311
 *
 *  Copyright (C) 2001-2007 Hovik Melikyan
 *
 *  http://www.melikyan.com/ptypes/
 *
 */

#include <JLJptypes.h>

#ifndef __PPORT_H__
#define __PPORT_H__

// 20090311, Guido Schmidt : a common define to distinguish 32 from 64 bit
// maybe not complete
#if defined(_WIN64) || defined(__64BIT__) || defined(__LP64__)  || defined(_LP64)
#  define PLATFORM_64
#  ifndef WIN64 
#    define WIN64
#  endif
#endif

#if defined(linux)
#  include <stdint.h>     // for uintptr_t
#endif

#include <sys/types.h>
#include <stddef.h>

#ifndef __cplusplus
#  error "This is a C++ source"
#endif


#define PTYPES_NAMESPACE pt

//
// conditional namespace declarations
//

#ifdef PTYPES_NAMESPACE
#  define PTYPES_BEGIN      namespace PTYPES_NAMESPACE {
#  define PTYPES_END        }
#  define USING_PTYPES      using namespace PTYPES_NAMESPACE;
#else
#  define PTYPES_NAMESPACE
#  define PTYPES_BEGIN
#  define PTYPES_END
#  define USING_PTYPES
#endif


//
// Windows DLL export/import and calling convention macros
//

#ifdef _WIN32
#  if defined(PTYPES_DLL_EXPORTS)
#    define ptpublic	 __declspec(dllexport)
#  elif defined(PTYPES_DLL)
#    define ptpublic	__declspec(dllimport)
#  else
#    define ptpublic
#  endif
#  define ptdecl	__stdcall
#  define __PFASTCALL __fastcall
#else
#  define ptpublic	
#  define ptdecl	
#  define __PFASTCALL
#endif


//
// versioning
//


extern "C" ptpublic unsigned long __ptypes_version;

// this enables old algebraic list interfaces; NO_PTYPES19_COMPAT
// can be defined at command line
#if !defined(NO_PTYPES19_COMPAT)
#  define PTYPES19_COMPAT
#endif


PTYPES_BEGIN


#ifdef _MSC_VER
// we don't want "unreferenced inline function" warning
#  pragma warning (disable: 4514)
// ... also "copy constructor/assignment operator could not be generated"
#  pragma warning (disable: 4511)
#  pragma warning (disable: 4512)
// disable deprecation warnings for snprintf and others
#  pragma warning (disable: 4996)
#endif

#if defined(_DEBUG) && !defined(DEBUG)
#  define DEBUG
#endif

#if defined(__WIN32__) && !defined(_WIN32)
#  define _WIN32
#endif

// __APPLE__ is the only predefined macro on MacOS X
#if defined(__APPLE__)
#  define __DARWIN__
#endif

// CHECK_BOUNDS enables bounds checking for strings and lists
#if defined(DEBUG) && !defined(CHECK_BOUNDS)
#  define CHECK_BOUNDS
#endif

// COUNT_OBJALLOC helps to keep track of the number of
// objects created/destroyed
#if defined(DEBUG) && !defined(COUNT_OBJALLOC)
#  define COUNT_OBJALLOC
#endif


//
// useful typedefs
//

typedef unsigned int    uint;
typedef unsigned long   ulong;
typedef unsigned short  ushort;
typedef unsigned char   uchar;
typedef signed char     schar;
typedef char*           pchar;
typedef const char*     pconst;
typedef void*           ptr;
typedef int*            pint;

#ifdef _WIN32
// 20090311, GSM : support 64bit
  typedef intptr_t     pintptr;
#else
  typedef uintptr_t    pintptr;
#endif


//
// portable 64-bit integers
//

#if defined(_MSC_VER) || defined(__BORLANDC__)
   typedef __int64             large;
   typedef unsigned __int64    ularge;
#  define LLCONST(a) (a##i64)
#else
   typedef long long           large;
   typedef unsigned long long  ularge;
#  define LLCONST(a) (a##ll)
#endif

#define LARGE_MIN (LLCONST(-9223372036854775807)-1)
#define LARGE_MAX (LLCONST(9223372036854775807))
#define ULARGE_MAX (LLCONST(18446744073709551615u))

#if defined(_MSC_VER) || defined(__BORLANDC__)
#  define strcasecmp stricmp
#  define snprintf _snprintf
#endif


//
// misc.
//

// I like Pascal's nil
#define nil 0

inline int   imax(int x, int y)       { return (x > y) ? x : y; }
inline int   imin(int x, int y)       { return (x < y) ? x : y; }
inline large lmax(large x, large y)   { return (x > y) ? x : y; }
inline large lmin(large x, large y)   { return (x < y) ? x : y; }


//
// critical error processing
//

#define CRIT_FIRST 0xC0000

typedef void (ptdecl *_pcrithandler)(int code, const char* msg);

ptpublic _pcrithandler ptdecl getcrithandler();
ptpublic _pcrithandler ptdecl setcrithandler(_pcrithandler newh);

ptpublic void ptdecl fatal(int code, const char* msg);


//
// memory management (undocumented)
// hides some BSD* incompatibility issues
//

ptpublic void* ptdecl memalloc(uint a);
ptpublic void* ptdecl memrealloc(void* p, uint a);
ptpublic void  ptdecl memfree(void* p);
ptpublic void  ptdecl memerror();
ptpublic int   ptdecl memquantize(int);


PTYPES_END


#endif // __PPORT_H__
