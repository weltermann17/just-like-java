// config.h

//********************************************************************

#ifndef config_h
#define config_h

//********************************************************************
//********************************************************************

#include <JLJptypes.h>
#include <JLJcore.h>
#include <JLJlang.h>
#include <JLJio.h>
#include <JLJutil.h>
#include <JLJxml.h>
#include <JLJnet.h>
#include <JLJnio.h>
#include <JLJjms.h>

//********************************************************************

#include <jlj/core/debug/assert.h>

//********************************************************************

#define MODERNCPP

//********************************************************************

#if defined (MODERNCPP)

	#define HASBOOL
	#define TEMPLSPEC template<> inline

//********************************************************************

#else

	#error "Your compiler is not supported anymore. Bad luck."

	#undef HASBOOL
	#define TEMPLSPEC inline
	#define TEMPLMEMB

//********************************************************************

#endif

//********************************************************************

#ifndef SIZEOFWCHAR_T
#define SIZEOFWCHAR_T 2
#endif

//********************************************************************

#ifndef NEEDTOCONVERT
#define NEEDTOCONVERT 0
#endif

//********************************************************************

/**
* more platform specifics if required
*/ 

#if defined (_AIX)

#if defined(__64BIT__)
#undef SIZEOFWCHAR_T
#define SIZEOFWCHAR_T 4
#endif

#undef USE_SMALL_OBJECT_ALLOCATOR

//********************************************************************

#elif defined (__hpux)

#undef SIZEOFWCHAR_T
#define SIZEOFWCHAR_T 4

//********************************************************************

#elif defined (sun)

	#include <widec.h>

//********************************************************************

#elif defined (_MSC_VER)

#undef NEEDTOCONVERT
#define NEEDTOCONVERT 1

#pragma warning ( disable: 4250 4291 4102 4275 4251 4786 4530 4541 )

#undef USE_SMALL_OBJECT_ALLOCATOR

#ifndef _CPPRTTI 
#define _CPPRTTI
#endif

//********************************************************************

#endif

//********************************************************************

// namespace handling (primitive)

#define NAMESPACE_BEGIN(name) namespace name {

#define NAMESPACE_END(name) } // name

//********************************************************************

#endif

// eof
