#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJjms
#define ExportedByJLJjms     __declspec(dllexport)
#else
#define ExportedByJLJjms     __declspec(dllimport)
#endif
#else
#define ExportedByJLJjms
#endif
