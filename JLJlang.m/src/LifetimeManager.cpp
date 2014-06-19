// LifetimeManager.cpp

//********************************************************************

#ifndef LifetimeManager_cpp
#define LifetimeManager_cpp

//********************************************************************

#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
#include <jlj/lang/lifetimemanagement/AbstractLifetimeImplementation.h>
#include <jlj/lang/ClassLoader.h>
#include <jlj/core/ptypes/pasync.h>
using namespace jlj::lang;
#include <stdio.h>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)
NAMESPACE_BEGIN(lifetimemanagement)

//********************************************************************

struct lessstring : public std::binary_function<std::string, std::string, bool>
{	
	bool operator()(const std::string& a, const std::string& b) const
	{
		return a < b;
	}
};

//********************************************************************

typedef std::map<std::string, LifetimeManager, lessstring> LifetimeManagers;
typedef std::map<std::string, jlj::lang::Object, lessstring> Objects;
typedef std::map<std::string, Objects*, lessstring> PreallocatedObjects;

//********************************************************************

LifetimeManagers& lifetimemanagers()
{
	static LifetimeManagers* lifetimemanagers_ = new LifetimeManagers();
	return *lifetimemanagers_;
}

//********************************************************************

PreallocatedObjects& preallocatedobjects()
{
	static PreallocatedObjects* preallocatedobjects_ = new PreallocatedObjects();
	return *preallocatedobjects_;
}

//********************************************************************

pt::rwlock& rwlock()
{
	static pt::rwlock* rwlock_ = new pt::rwlock();
	return *rwlock_;
}

//********************************************************************
//********************************************************************

LifetimeManagerI::~LifetimeManagerI() throw (Exception) 
{
	std::string n(constcharptr(name));
	if (preallocatedobjects().end() != preallocatedobjects().find(n))
	{
		Objects* objects = preallocatedobjects()[n];
		delete objects;
		preallocatedobjects().erase(preallocatedobjects().find(n));
	}
}

//********************************************************************

LifetimeManagerI::LifetimeManagerI(const String& name) throw (Exception)
: name(name)
{
	pt::scopewrite scope(rwlock());
	std::string n(constcharptr(name));
	lifetimemanagers()[n] = this;
	preallocatedobjects()[n] = new Objects();
}

//********************************************************************

bool LifetimeManagerI::containsKey(const String& key)
{
	pt::scoperead scope(rwlock());
	Objects* objects = preallocatedobjects()[std::string(constcharptr(name))];
	return (objects->end() != objects->find(constcharptr(key)));
}

//********************************************************************

bool LifetimeManagerI::putObject(const String& key, const Object& value)
{
	pt::scopewrite scope(rwlock());
	Objects* objects = preallocatedobjects()[std::string(constcharptr(name))];
	std::string k(constcharptr(key));
	(*objects)[k] = value;
	return objects->end() != objects->find(k);
}

//********************************************************************

Object LifetimeManagerI::getObject(const String& key) throw (Exception)
{
	pt::scoperead scope(rwlock());
	Objects* objects = preallocatedobjects()[std::string(constcharptr(name))];
	std::string k(constcharptr(key));
	if (objects->end() == objects->find(k)) throw IndexOutOfBoundsException(WITHDETAILS(L"Preallocated object not found : " + key));
	return (*objects)[k];
}

//********************************************************************

void LifetimeManagerI::remove(const String& key) throw (Exception)
{
	pt::scopewrite scope(rwlock());
	Objects* objects = preallocatedobjects()[std::string(constcharptr(name))];
	std::string k(constcharptr(key));
	if (objects->end() == objects->find(k)) throw IndexOutOfBoundsException(WITHDETAILS(L"Preallocated object cannot be removed : " + key));
	objects->erase(objects->find(k));
}

//********************************************************************

void LifetimeManagerI::addInstance(const String& name,
								   LifetimeManager lifetimemanager) throw (Exception)
{
	pt::scopewrite scope(rwlock());
	std::string n(constcharptr(name));
	lifetimemanagers()[n] = lifetimemanager;
}

//********************************************************************

void LifetimeManagerI::removeInstance(const String& name) throw (Exception)
{
	pt::scopewrite scope(rwlock());
	std::string n(constcharptr(name));
	if (lifetimemanagers().end() == lifetimemanagers().find(n)) 
		throw  IndexOutOfBoundsException(WITHDETAILS(L"LifetimeManager not found : " + name));
	lifetimemanagers().erase(lifetimemanagers().find(n));
}

//********************************************************************

LifetimeManager LifetimeManagerI::getInstance(const String& name) throw (Exception)
{
	pt::scoperead scope(rwlock());
	std::string n(constcharptr(name));
	if (lifetimemanagers().end() == lifetimemanagers().find(n)) 
		throw IndexOutOfBoundsException(WITHDETAILS(L"LifetimeManager not found : " + name));
	return lifetimemanagers()[n];
}

//********************************************************************
//********************************************************************

LifetimeManager LifetimeManager::getInstance(const String& name) throw (Exception)
{
	return LifetimeManagerI::getInstance(name);
}

//********************************************************************

NAMESPACE_END(lifetimemanagement)
NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
