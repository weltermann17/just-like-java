#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJlang
#define ExportedByJLJlang     __declspec(dllexport)
#else
#define ExportedByJLJlang     __declspec(dllimport)
#endif
#else
#define ExportedByJLJlang
#endif

#include <JLJio.h>