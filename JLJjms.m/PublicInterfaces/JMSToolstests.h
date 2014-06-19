#ifdef  _WINDOWS_SOURCE
#ifdef  __JMSToolstests
#define ExportedByJMSToolstests     __declspec(dllexport)
#else
#define ExportedByJMSToolstests     __declspec(dllimport)
#endif
#else
#define ExportedByJMSToolstests
#endif
