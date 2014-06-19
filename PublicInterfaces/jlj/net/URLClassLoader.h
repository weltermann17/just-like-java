// URLClassLoader.h

//********************************************************************

#ifndef URLClassLoader_h
#define URLClassLoader_h

//********************************************************************

#include <jlj/lang/ClassLoader.h>
#include <jlj/util/Map.h>
using namespace jlj::lang;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet URLClassLoaderI
	: public ClassLoaderI
{
protected:
	~URLClassLoaderI();
	friend class TypedReference<URLClassLoaderI>;

public:
	URLClassLoaderI() throw (Exception);

public:
	String loadLibrary(const String& url) throw (Exception);
	bool unloadLibrary(const String& url) throw (Exception);
	Class loadClass(const String& clazzname) throw (ClassNotFoundException);

private:
	void removeFile(const String& filepath) throw (Exception);
	void moveFile(const String& from, const String& to) throw (Exception);
	bool fileExists(const String& filepath) const throw (Exception);

	ClassLoader classloader;
	Map<String, String> loadedlibraries;
	String librarycache;
};

//********************************************************************

StrongReferenceCommonDeclaration(URLClassLoader, ExportedByJLJnet);
ClassLoaderInformationDeclaration(jlj::net::URLClassLoader, 0abdee87ce013e760273d50428749840, ExportedByJLJnet); 

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
