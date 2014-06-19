#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJcore
#define ExportedByJLJcore     __declspec(dllexport)
#else
#define ExportedByJLJcore     __declspec(dllimport)
#endif
#else
#define ExportedByJLJcore
#endif
