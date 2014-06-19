#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJutil
#define ExportedByJLJutil     __declspec(dllexport)
#else
#define ExportedByJLJutil     __declspec(dllimport)
#endif
#else
#define ExportedByJLJutil
#endif
