// Throwable.h

//********************************************************************

#ifndef Throwable_h
#define Throwable_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/StringBuffer.h> 

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang ThrowableI
	: public ObjectI
{
protected:
	~ThrowableI();
	friend class TypedReference<ThrowableI>;

public:
	ThrowableI();
	ThrowableI(const String&);

public:
	bool equals(const Object&) const ;	
	int hashCode() const;
	String toString() const;

	String getMessage() const;

protected:
	String message;

};

//********************************************************************

StrongReferenceCommonDeclaration(Throwable, ExportedByJLJlang);

//********************************************************************

#define WITHDETAILS(message) String(StringBuffer(message)->append(L"\nfile : ")->append(String(__FILE__, strlen(__FILE__)))->append(L", line : ")->append(__LINE__)) 

//********************************************************************

#if !defined(TRY)
#define TRY try {
#else
#undef TRY
#define TRY try {
#endif

//********************************************************************

#if !defined(CATCH)
#define CATCH(exceptiontype) } catch (...) { throw exceptiontype(WITHDETAILS(L"")); }
#else
#undef CATCH
#define CATCH(exceptiontype) } catch (...) { throw exceptiontype(WITHDETAILS(L"")); }
#endif

//********************************************************************

#define DeriveException(exportedby, classname, basename) \
	\
class exportedby classname##I \
	: public basename##I \
{ \
public: \
	classname##I() : basename##I() {} \
	classname##I(const jlj::lang::String& m) : basename##I(m) {} \
}; \
	\
class exportedby classname \
	: public basename \
{ \
public: \
	classname() : basename(new classname##I()) {} \
	classname(const jlj::lang::String& m) : basename(new classname##I(m)) {} \
	explicit classname(classname##I* p) : basename(p) {} \
} \
	\

//********************************************************************
//********************************************************************

DeriveException(ExportedByJLJlang, Exception, Throwable);
DeriveException(ExportedByJLJlang, RuntimeException, Exception);
DeriveException(ExportedByJLJlang, InterruptedException, Exception);
DeriveException(ExportedByJLJlang, NullPointerException, RuntimeException);
DeriveException(ExportedByJLJlang, IndexOutOfBoundsException, RuntimeException);
DeriveException(ExportedByJLJlang, IllegalArgumentException, RuntimeException);
DeriveException(ExportedByJLJlang, ClassCastException, RuntimeException);
DeriveException(ExportedByJLJlang, ClassNotFoundException, RuntimeException);
DeriveException(ExportedByJLJlang, InstantiationException, RuntimeException);
DeriveException(ExportedByJLJlang, UnsupportedOperationException, RuntimeException);
DeriveException(ExportedByJLJlang, IllegalStateException, RuntimeException);
DeriveException(ExportedByJLJlang, IllegalMonitorStateException, RuntimeException);
DeriveException(ExportedByJLJlang, NoSuchElementException, RuntimeException);

//********************************************************************
//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
