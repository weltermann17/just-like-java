#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJnio
#define ExportedByJLJnio     __declspec(dllexport)
#else
#define ExportedByJLJnio     __declspec(dllimport)
#endif
#else
#define ExportedByJLJnio
#endif
