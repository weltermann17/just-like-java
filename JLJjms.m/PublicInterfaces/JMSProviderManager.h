#ifdef  _WINDOWS_SOURCE
#ifdef  __JMSProviderManager
#define ExportedByJMSProviderManager     __declspec(dllexport)
#else
#define ExportedByJMSProviderManager     __declspec(dllimport)
#endif
#else
#define ExportedByJMSProviderManager
#endif
