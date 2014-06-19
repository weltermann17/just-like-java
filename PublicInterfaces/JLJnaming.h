#ifdef  _WINDOWS_SOURCE
#ifdef  __JLJnaming
#define ExportedByJLJnaming     __declspec(dllexport)
#else
#define ExportedByJLJnaming     __declspec(dllimport)
#endif
#else
#define ExportedByJLJnaming
#endif
