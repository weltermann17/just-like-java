#ifdef  _WINDOWS_SOURCE
#ifdef  __JMSTOOLSservicesprovider
#define ExportedByJMSTOOLSservicesprovider     __declspec(dllexport)
#else
#define ExportedByJMSTOOLSservicesprovider     __declspec(dllimport)
#endif
#else
#define ExportedByJMSTOOLSservicesprovider
#endif
