// LocaleGuard.cpp

//********************************************************************

#ifndef LocaleGuard_cpp
#define LocaleGuard_cpp

//********************************************************************

#include <jlj/core/platform/LocaleGuard.h>

#ifndef _WINDOWS_SOURCE
#include <string.h>
#endif
//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(platform)

//********************************************************************

LocaleGuard::~LocaleGuard()
{
	setlocale(LC_NUMERIC, oldlocale);
}

//********************************************************************

LocaleGuard::LocaleGuard()
{
	strcpy(oldlocale, setlocale(LC_NUMERIC, 0));
	setlocale(LC_NUMERIC, "C");
}

//********************************************************************

LocaleGuard::LocaleGuard(const char* locale)
{
	strcpy(oldlocale, setlocale(LC_NUMERIC, 0));
	setlocale(LC_NUMERIC, locale);
}

//********************************************************************

NAMESPACE_END(platform)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
