// jlj_lang_Extension.h

//********************************************************************

#ifndef jlj_lang_Extension
#define jlj_lang_Extension

//********************************************************************
   
#include <jlj/lang/Object.h>
#include <jlj/lang/Throwable.h>

//********************************************************************			   

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang ExtensionI 
	: public virtual ObjectI
{
public:	
	virtual ~ExtensionI() = 0;

public:
	virtual Object extend(Object) throw (Exception) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Extension, ExportedByJLJlang);

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
