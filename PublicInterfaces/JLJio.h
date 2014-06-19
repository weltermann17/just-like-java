#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJio
#define ExportedByJLJio     __declspec(dllexport)
#else
#define ExportedByJLJio     __declspec(dllimport)
#endif
#else
#define ExportedByJLJio
#endif
