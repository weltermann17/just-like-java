#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJxml
#define ExportedByJLJxml     __declspec(dllexport)
#else
#define ExportedByJLJxml     __declspec(dllimport)
#endif
#else
#define ExportedByJLJxml
#endif
