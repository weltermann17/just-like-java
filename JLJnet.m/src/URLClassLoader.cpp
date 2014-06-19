// URLClassLoader.cpp

//********************************************************************

#ifndef URLClassLoader_cpp
#define URLClassLoader_cpp				

//********************************************************************

#include <jlj/net/URLClassLoader.h> 
#include <jlj/lang/UTF8StringBuffer.h> 
#include <jlj/lang/Extension.h> 
#include <jlj/util/LinkedHashMap.h> 
#include <jlj/util/SessionDirectory.h> 
#include <jlj/util/Uuid.h> 
#include <jlj/net/Url.h> 
#include <jlj/net/HttpURLConnection.h> 
#if defined(_WINDOWS_SOURCE) 
#include <direct.h>
#include <io.h>
#endif

#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

ClassLoaderInformationDefinition(jlj::net::URLClassLoader, 0abdee87ce013e760273d50428749840) 

//********************************************************************

URLClassLoaderI::~URLClassLoaderI()
{}

//********************************************************************

URLClassLoaderI::URLClassLoaderI() throw (Exception) 
: classloader(new ClassLoaderI())
, loadedlibraries(new LinkedHashMapI<String, String>())
{
	ClassLoader classloader = new ClassLoaderI();
	classloader->loadLibrary(L"JLJnaming");
	Extension extension;
	classloader->loadClass(L"jlj::naming::URLClassLoaderExtension")->newInstance()->downcast(extension);
	Anything any;
	extension->extend(any)->downcast(any);
	librarycache = any;
}

//********************************************************************

String URLClassLoaderI::loadLibrary(const String& urlstring) throw (Exception)
{
	if (loadedlibraries->containsKey(urlstring)) return loadedlibraries->containsKey(urlstring);

	String filepath;
	if (0 < librarycache->length())
	{
#if defined(_WINDOWS_SOURCE) 
		_mkdir(constcharptr(librarycache->toUTF8String()));
#else
		mkdir(constcharptr(librarycache->toUTF8String()), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
		filepath = librarycache + urlstring->toMD5() + L".tmp";
		if (!fileExists(filepath)) filepath = L"";
	}

	if (0 == filepath->length())
	{
		URL url = new URLI(urlstring);
		HttpURLConnection httpurlconnection;
		url->openConnection()->downcast(httpurlconnection);
		int rc = httpurlconnection->getResponseCode();
		if (200 != rc) throw IOException(WITHDETAILS(L"Could not GET: " + urlstring + L", errorcode : " + String(rc)));
		String responsefilepath = httpurlconnection->getResponseFilePath();
		if (0 < librarycache->length())
		{
			filepath = librarycache + urlstring->toMD5() + L".tmp";
			moveFile(responsefilepath, filepath);
		}
		else
		{
			filepath = responsefilepath;
		}
	}

	classloader->loadLibrary(filepath);
	loadedlibraries->put(urlstring, filepath);
	return filepath;
}

//********************************************************************

bool URLClassLoaderI::unloadLibrary(const String& url) throw (Exception)
{
	if (!loadedlibraries->containsKey(url)) return false;
	String filepath = loadedlibraries->get(url);
	loadedlibraries->remove(url);
	removeFile(filepath);
	return classloader->unloadLibrary(filepath);
}

//********************************************************************

Class URLClassLoaderI::loadClass(const String& clazzname) throw (ClassNotFoundException)
{
	return classloader->loadClass(clazzname);
}

//********************************************************************

void URLClassLoaderI::removeFile(const String& filepath) throw (Exception)
{
	remove(constcharptr(filepath->toUTF8String()));
}

//********************************************************************

bool URLClassLoaderI::fileExists(const String& filepath) const throw (Exception)
{
#if defined(_WINDOWS_SOURCE) 
	return 0 == _access(constcharptr(filepath->toUTF8String()), 0);
#else
	return 0 == access(constcharptr(filepath->toUTF8String()), 0);
#endif
}

//********************************************************************

void URLClassLoaderI::moveFile(const String& from, const String& to) throw (Exception)
{
	UTF8StringBuffer commandline;
#if defined(_WINDOWS_SOURCE) 
	commandline->append(getenv("COMSPEC"));
	commandline->append(" /c move /y \"");
	commandline->append(from);
	commandline->append("\" \"");
	commandline->append(to);
	commandline->append("\" > nul 2> nul");
#else
	commandline->append("mv -f \"");
	commandline->append(from);
	commandline->append("\" \"");
	commandline->append(to);
	commandline->append("\" 1> /dev/null 2>@1");
#endif
	system(constcharptr(commandline));
}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
