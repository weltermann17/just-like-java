// LocaleGuard.h

//********************************************************************

#ifndef LocaleGuard_h
#define LocaleGuard_h

//********************************************************************

#include <jlj/core/platform/config.h>
#include <locale.h>
#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(platform)

//********************************************************************

class ExportedByJLJcore LocaleGuard
{
public:
	~LocaleGuard();
	LocaleGuard();
	LocaleGuard(const char* localename);
	
private:
	char oldlocale[256];

};

//********************************************************************

NAMESPACE_END(platform)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
