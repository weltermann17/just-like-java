// Class.h

//********************************************************************

#ifndef Class_h
#define Class_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Throwable.h>
#include <jlj/lang/String.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang ClassLoaderI;

//********************************************************************

class ExportedByJLJlang ClassI
	: public ObjectI
{
protected:
	~ClassI();
	friend class TypedReference<ClassI>;

public:
	ClassI(const String&);
	ClassI(const char*);

public:
	bool equals(const Object&) const ;	
	int hashCode() const;
	String toString() const;
	String getName() const;

	Object newInstance() throw (InstantiationException);

	static String normalizeClassName(const char*);

private:
	String clazzname;

};

//********************************************************************

StrongReferenceCommonDeclaration(Class, ExportedByJLJlang);

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
