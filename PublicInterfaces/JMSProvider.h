#ifdef  _WINDOWS_SOURCE
#ifdef  __JMSProvider
#define ExportedByJMSProvider     __declspec(dllexport)
#else
#define ExportedByJMSProvider     __declspec(dllimport)
#endif
#else
#define ExportedByJMSProvider
#endif
