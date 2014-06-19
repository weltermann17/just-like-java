// bigendian.h

//********************************************************************

#ifndef bigendian_h
#define bigendian_h

//********************************************************************

#include <jlj/core/platform/config.h>
#include <wchar.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(platform)

//********************************************************************

ExportedByJLJcore bool needToConvertBigEndian();

//********************************************************************

ExportedByJLJcore short bigendian(short littleendian);

//********************************************************************

ExportedByJLJcore wchar_t bigendian(wchar_t littleendian);

//********************************************************************

ExportedByJLJcore int bigendian(int littleendian);

//********************************************************************

ExportedByJLJcore long bigendian(long littleendian);

//********************************************************************

ExportedByJLJcore double bigendian(double littleendian);

//********************************************************************

ExportedByJLJcore const char* getCurrentProcessName();

//********************************************************************

NAMESPACE_END(platform)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
