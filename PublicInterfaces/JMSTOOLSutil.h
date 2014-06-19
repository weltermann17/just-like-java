#ifdef  _WINDOWS_SOURCE
#ifdef  __JMSTOOLSutil
#define ExportedByJMSTOOLSutil     __declspec(dllexport)
#else
#define ExportedByJMSTOOLSutil     __declspec(dllimport)
#endif
#else
#define ExportedByJMSTOOLSutil
#endif
