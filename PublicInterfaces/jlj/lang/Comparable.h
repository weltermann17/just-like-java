// Comparable.h

//********************************************************************

#ifndef jlj_lang_Comparable_h
#define jlj_lang_Comparable_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang ComparableI
	: public virtual ObjectI
{
public:
	virtual ~ComparableI() = 0;
	ComparableI();

public:
	virtual int compareTo(const Object&) const = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Comparable, ExportedByJLJlang);

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
