#ifdef  _WINDOWS_SOURCE
#ifdef  __JMSTOOLSTESTservicesprovider
#define ExportedByJMSTOOLSTESTservicesprovider     __declspec(dllexport)
#else
#define ExportedByJMSTOOLSTESTservicesprovider     __declspec(dllimport)
#endif
#else
#define ExportedByJMSTOOLSTESTservicesprovider
#endif
