// ClassLoader.h

//********************************************************************

#ifndef ClassLoader_h
#define ClassLoader_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
#include <jlj/lang/Class.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang ClassLoaderI
	: public ObjectI
{
protected:
	~ClassLoaderI();
	friend class TypedReference<ClassLoaderI>;

public:
	ClassLoaderI() throw (Exception);
	ClassLoaderI(const String&) throw (Exception);

public:
	/**
	* libraryname: like in link -lXXX
	* XXX will be resolved to XXX.dll on Windows and libXXX.a or libXXX.so on unix
	* unless an extension is explicitely given
	*/

	virtual String loadLibrary(const String& libraryname) throw (Exception);
	virtual String loadLibrary(const String& libraryname, const String& extension) throw (Exception);
	virtual bool unloadLibrary(const String& libraryname) throw (Exception);
	virtual Class loadClass(const String& clazzname) throw (ClassNotFoundException);

private:
	friend class ExportedByJLJlang ClassI;
	Object newInstance(const String& clazzname) throw (Exception); 

};

//********************************************************************

/**
* convert the full classname (eg. jlj::lang::ClassLoader) to lowercase/nofillers MD5 (eg. at http://www.iwebtool.com/md5)
* or call aninstanceofmyclass->getClass()->getName()->toMD5() on an instance of your class
*/

StrongReferenceCommonDeclaration(ClassLoader, ExportedByJLJlang);
ClassLoaderInformationDeclaration(jlj::lang::ClassLoader, 519f6e80824c10f0e41d6ecf82a1951d, ExportedByJLJlang); 

//********************************************************************

ExportedByJLJlang void initializeClassLoader() throw (Exception);
ExportedByJLJlang void finalizeClassLoader() throw (Exception); 

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
