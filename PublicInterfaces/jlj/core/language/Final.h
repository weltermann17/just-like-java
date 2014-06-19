// Final.h

//********************************************************************

#ifndef Final_h
#define Final_h

//********************************************************************

#include <jlj/core/platform/config.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(language)

//********************************************************************

/**
* to make a class A "final" (as in Java) just add the macro "FINAL(A)" 
* to its inheritance list; making it final prevents users inheriting from it
*
* eg:
*		class A : FINAL(A) { ... ... }
*
* or with more super classes
*
*		class A : public FatherX, public FatherY, FINAL(A) { ... ... }
*
*/

//********************************************************************

#define FINALC(classname) \
class classname##IsFinal \
{ \
private: \
	friend class classname; \
	classname##IsFinal() {} \
} \

//********************************************************************

#define FINAL(classname) \
	virtual private classname##IsFinal 

//********************************************************************

NAMESPACE_END(language)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

//********************************************************************

// eof
