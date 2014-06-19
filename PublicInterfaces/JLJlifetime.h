#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJlifetime
#define ExportedByJLJlifetime     __declspec(dllexport)
#else
#define ExportedByJLJlifetime     __declspec(dllimport)
#endif
#else
#define ExportedByJLJlifetime
#endif
