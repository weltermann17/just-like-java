/* hashfunc.h */

//********************************************************************

#ifndef hashfunc_h
#define hashfunc_h

//********************************************************************

#include <jlj/core/platform/config.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(hashing)

//********************************************************************

/**
* lookup2.c, by Bob Jenkins, December 1996, Public Domain.
* hash(), hash2(), hash3, and mix() are externally useful functions.
* Routines to test the hash are included if SELF_TEST is defined.
* You can use this free for any purpose.  It has no warranty.
*/

//********************************************************************

ExportedByJLJcore
extern unsigned long int hash1(register unsigned char *k,				
							   register unsigned long length,		
							   register unsigned long step,		
							   register unsigned long initvalue);		

//********************************************************************

ExportedByJLJcore
extern unsigned long int hash2(register unsigned long *k,				
							   register unsigned long length,		
							   register unsigned long step,		
							   register unsigned long initvalue);		

//********************************************************************

ExportedByJLJcore 
extern const int HASHMAXSIGNIFICANTLENGTH;

//********************************************************************

NAMESPACE_END(hashing)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif 

// eof
