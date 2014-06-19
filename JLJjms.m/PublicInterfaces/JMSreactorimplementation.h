#ifdef  _WINDOWS_SOURCE
#ifdef  __JMSreactorimplementation
#define ExportedByJMSreactorimplementation     __declspec(dllexport)
#else
#define ExportedByJMSreactorimplementation     __declspec(dllimport)
#endif
#else
#define ExportedByJMSreactorimplementation
#endif
