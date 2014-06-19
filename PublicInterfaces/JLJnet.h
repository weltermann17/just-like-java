#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJnet
#define ExportedByJLJnet     __declspec(dllexport)
#else
#define ExportedByJLJnet     __declspec(dllimport)
#endif
#else
#define ExportedByJLJnet
#endif
