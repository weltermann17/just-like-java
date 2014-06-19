// ClassLoader.cpp

//********************************************************************

#ifndef ClassLoader_cpp
#define ClassLoader_cpp				

//********************************************************************

#include <jlj/lang/ClassLoader.h> 
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::lang::lifetimemanagement;
#include <jlj/lang/StringBuffer.h> 
#if defined(_MSC_VER) 
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <map>
#include <iostream>
#include <string>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************
//********************************************************************

class ClassLoaderImplementationI
	: public ObjectI
{
public:
	~ClassLoaderImplementationI() throw (Exception);
	ClassLoaderImplementationI();

	String loadLibrary(const String& libraryname, const String& extension) throw (Exception);
	bool unloadLibrary(const String& libraryname) throw (Exception);
	Class loadClass(const String& clazzname) throw (ClassNotFoundException);
	Object newInstance(const String& clazzname) throw (Exception);

private:
	bool isLoaded(const String& libraryname) const;
	std::string fullName(const String& libraryname, const String& extension) const;
	void* findInstantiationFunction(const String& libraryname) throw (Exception);
	std::map<std::string, void*> loadedlibraries;

};

//********************************************************************

ClassLoaderImplementationI::~ClassLoaderImplementationI() throw (Exception)	        
{
#if 0
	for (std::map<std::string, void*>::iterator i = loadedlibraries.end(); i != loadedlibraries.begin(); )
	{
		--i;
		void* libraryhandle = i->second;
		if (0 != libraryhandle)
		{
			const char* name = i->first.c_str();
			// std::cout << "~ClassLoader() : " << loadedlibraries.size() << " : " << (0 != name ? name : "null") << " = " << libraryhandle << std::endl;

#if defined(_MSC_VER) 
			FreeLibrary((HINSTANCE) libraryhandle);
#else 
			dlclose(libraryhandle);
#endif
		}
	}
#endif
}

//********************************************************************

ClassLoaderImplementationI::ClassLoaderImplementationI()
{}

//********************************************************************

String ClassLoaderImplementationI::loadLibrary(const String& libraryname, 
											   const String& extension) throw (Exception)
{
	try
	{
		if (isLoaded(libraryname))
		{
			return libraryname;
		}
		else
		{
			std::string name(fullName(libraryname, extension));
			void* libraryhandle = 0;

#if defined(_MSC_VER) 
			libraryhandle = (void*) LoadLibraryA(name.c_str());
			if (0 == libraryhandle) throw Exception(L"loadLibrary() failed : " + libraryname + L", errorcode : " + String((int) GetLastError()));
#else 
			libraryhandle = dlopen(name.c_str(), RTLD_NOW);
			if (0 == libraryhandle) throw Exception(L"loadLibrary() failed : " + libraryname + L", errorcode : " + String(dlerror()));
#endif
			std::string shortname(constcharptr(libraryname));

#if !defined(_MSC_VER) 
			if (L"JLJnaming" != libraryname) 
#endif
			{
				loadedlibraries[shortname] = libraryhandle;
				// std::cout << "ClassLoader::loadLibrary() : " << loadedlibraries.size() << " : " << (0 != shortname ? shortname : "null") << " = " << libraryhandle << std::endl;
			}

			return libraryname;
		}
	}
	catch (const Exception&)
	{
		throw;
	}
	catch (...)
	{
		throw Exception(WITHDETAILS(L"loadLibrary() failed."));
	}

}

//********************************************************************

bool ClassLoaderImplementationI::unloadLibrary(const String& libraryname) throw (Exception)
{
	try
	{
		bool success = false;
		void* libraryhandle = 0;
		std::string name(constcharptr(libraryname));
		if (isLoaded(libraryname)) libraryhandle = loadedlibraries[name]; 
		if (0 != libraryhandle)
		{
#if defined(_MSC_VER) 
			success = 0 != FreeLibrary((HINSTANCE) libraryhandle);
#else 
			success = 0 == dlclose(libraryhandle);
#endif
			if (success) loadedlibraries.erase(loadedlibraries.find(name));
			// std::cout << "ClassLoader::unloadLibrary() : " << loadedlibraries.size() << " : " << name << std::endl;
		}
		return success;
	}
	catch (...)
	{
		throw Exception(WITHDETAILS(L"unloadLibrary() failed."));
	}
}

//********************************************************************

Class ClassLoaderImplementationI::loadClass(const String& clazzname) throw (ClassNotFoundException)
{
	try
	{
		findInstantiationFunction(clazzname);
		return new jlj::lang::ClassI(clazzname);
	}
	catch (const ClassNotFoundException&)
	{
		throw;
	}
	catch (const Exception& e)
	{
		throw ClassNotFoundException(e->toString());
	}
	catch (...)
	{
		throw ClassNotFoundException(WITHDETAILS(L"loadClass() failed"));
	}
}

//********************************************************************

Object ClassLoaderImplementationI::newInstance(const String& clazzname) throw (Exception)
{
	try
	{
		void* symbolhandle = findInstantiationFunction(clazzname);
		typedef jlj::lang::ObjectI* (*InstantiateFunction)();
		InstantiateFunction instantiatefunction = (InstantiateFunction) symbolhandle;
		if (0 == instantiatefunction) throw InstantiationException(clazzname + L" found, but not a valid factory method.");
		return instantiatefunction();
	}
	catch (const Exception&)
	{
		throw;
	}
	catch (...)
	{
		throw ClassNotFoundException(WITHDETAILS(L"loadClass() failed"));
	}
}

//********************************************************************

void* ClassLoaderImplementationI::findInstantiationFunction(const String& clazzname) throw (Exception)
{
	try
	{
		std::string functionname("instantiate_");
		functionname.append(constcharptr(clazzname->toMD5()));
		StringBuffer alllibraries;
		void* symbolhandle = 0;
		for (std::map<std::string, void*>::iterator i = loadedlibraries.begin(); i != loadedlibraries.end(); ++i)
		{
			alllibraries->append(L"\n");
			alllibraries->append(i->first);
#if defined(_MSC_VER)
			symbolhandle = (void*) GetProcAddress((HINSTANCE) i->second, functionname.c_str());
#else 
			symbolhandle = dlsym(i->second, functionname.c_str());
#endif
			if (0 != symbolhandle) break;
		}   
		if (0 == symbolhandle)
		{
#if defined(_MSC_VER)
			symbolhandle = (void*) GetProcAddress(GetModuleHandle(0), functionname.c_str());
#else 
			symbolhandle = dlsym(RTLD_MYSELF, functionname.c_str());
#endif
		}
		if (0 != symbolhandle)
		{
			return symbolhandle;
		}
		else
		{
			throw ClassNotFoundException(
				clazzname + L" (" + String(functionname.c_str()) + L") not found in these loaded libraries:" + alllibraries->toString()); 
		}
	}
	catch (const Exception&)
	{
		throw;
	}
	catch (...)
	{
		throw ClassNotFoundException(WITHDETAILS(L"loadClass() failed"));
	}
}

//********************************************************************

bool ClassLoaderImplementationI::isLoaded(const String& libraryname) const
{
	return loadedlibraries.find(constcharptr(libraryname)) != loadedlibraries.end(); 
}

//********************************************************************

std::string ClassLoaderImplementationI::fullName(const String& libraryname, 
												 const String& extension) const
{
	std::string prefix;
	std::string ext;
	StringBuffer buf; buf->append(libraryname);
#if defined(_MSC_VER) 
	if (-1 == buf->indexOf(L"\\")) 
	{
		ext = 0 < extension->length() ? constcharptr(extension) : ".dll";
#else
	if (-1 == buf->indexOf(L"/")) 
	{
		prefix = "lib";
		ext = 0 < extension->length() ? constcharptr(extension) : ".a";
#endif
	}
	return prefix + std::string(constcharptr(libraryname)) + ext;
}

//********************************************************************

#define DUMMY
StrongReferenceCommonDeclaration(ClassLoaderImplementation, DUMMY); 

//********************************************************************
//********************************************************************

void initializeClassLoader() throw (Exception) 
{
	LifetimeManager lifetimemanager = LifetimeManager::getInstance(L"jlj");
	if (!lifetimemanager->containsKey(L"classloaderimplementation"))
	{
		ClassLoaderImplementation cli = new ClassLoaderImplementationI();
		lifetimemanager->put(L"classloaderimplementation", cli);
	}
}

//********************************************************************

void finalizeClassLoader() throw (Exception) 
{
	LifetimeManager lifetimemanager = LifetimeManager::getInstance(L"jlj");
	if (lifetimemanager->containsKey(L"classloaderimplementation"))
	{
		lifetimemanager->remove(L"classloaderimplementation");
	}
}

//********************************************************************
//********************************************************************

ClassLoaderInformationDefinition(jlj::lang::ClassLoader, 519f6e80824c10f0e41d6ecf82a1951d) 

//********************************************************************

ClassLoaderI::~ClassLoaderI()
{}

//********************************************************************

ClassLoaderI::ClassLoaderI() throw (Exception) 
{}

//********************************************************************

ClassLoaderI::ClassLoaderI(const String&) throw (Exception)
{}

//********************************************************************

String ClassLoaderI::loadLibrary(const String& libraryname) throw (Exception)
{
	ClassLoaderImplementation cli;
	LifetimeManager::getInstance(L"jlj")->get(L"classloaderimplementation", cli);
	return cli->loadLibrary(libraryname, L"");
}

//********************************************************************

String ClassLoaderI::loadLibrary(const String& libraryname, 
								 const String& extension) throw (Exception)
{
	ClassLoaderImplementation cli;
	LifetimeManager::getInstance(L"jlj")->get(L"classloaderimplementation", cli);
	return cli->loadLibrary(libraryname, extension);
}

//********************************************************************

bool ClassLoaderI::unloadLibrary(const String& libraryname) throw (Exception)
{
	ClassLoaderImplementation cli;
	LifetimeManager::getInstance(L"jlj")->get(L"classloaderimplementation", cli);
	return cli->unloadLibrary(libraryname);
}

//********************************************************************

Class ClassLoaderI::loadClass(const String& clazzname) throw (ClassNotFoundException)
{
	ClassLoaderImplementation cli;
	LifetimeManager::getInstance(L"jlj")->get(L"classloaderimplementation", cli);
	return cli->loadClass(clazzname);
}

//********************************************************************

Object ClassLoaderI::newInstance(const String& clazzname) throw (Exception)
{
	ClassLoaderImplementation cli;
	LifetimeManager::getInstance(L"jlj")->get(L"classloaderimplementation", cli);
	return cli->newInstance(clazzname);
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof