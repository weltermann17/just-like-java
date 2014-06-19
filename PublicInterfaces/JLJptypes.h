#ifdef  _WINDOWS_SOURCE
#pragma warning( disable: 4530 )
#define NO_CRIT_MSGBOX
#ifdef  __JLJptypes
#define PTYPES_DLL_EXPORTS
#else
#define PTYPES_DLL
#endif
#else
#define _FILE_OFFSET_BITS=64
#endif
#define NO_PTYPES19_COMPAT
