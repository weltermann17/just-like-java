// assert.h

//********************************************************************

#ifndef assert_h
#define assert_h

//********************************************************************

#define NASSERTDEBUG // switch off any calls to ASSERT

#ifndef NASSERTDEBUG

//********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//********************************************************************

#ifndef ASSERT
#define ASSERT(expr) do { if (!(expr) && (1 < fprintf(stderr, "Assertion failed (fatal).\nFile:\t%s\nLine:\t%d\nExpr:\t'%s' is false\n", __FILE__, __LINE__, #expr))) raise(SIGABRT); } while (0)
#endif

//********************************************************************

#ifndef RETURN
#define RETURN(expr) do { if ((expr) && (1 < fprintf(stderr, "File:\t%s\nLine:\t%d\n", __FILE__, __LINE__))) (void*)0; } while (0); return (expr)
#endif

//********************************************************************

#else // NASSERTDEBUG

#ifndef ASSERT
#define ASSERT(expr) (void*)0
#endif

//********************************************************************

#ifndef RETURN
#define RETURN(expr) return (expr)
#endif

//********************************************************************

#endif

//********************************************************************

#ifndef STICKYASSERT
#define STICKYASSERT(expr) do { if (!(expr) && (1 < fprintf(stderr, "Assertion failed (non-fatal).\nFile:\t%s\nLine:\t%d\nExpr:\t'%s' is false\n", __FILE__, __LINE__, #expr))) (void*)0; } while (0)
#endif

//********************************************************************

#endif

// eof
