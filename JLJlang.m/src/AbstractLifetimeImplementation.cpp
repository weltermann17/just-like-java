// AbstractLifetimeImplementation.cpp

//********************************************************************

#ifndef AbstractLifetimeImplementation_cpp
#define AbstractLifetimeImplementation_cpp

//********************************************************************

#include <jlj/lang/lifetimemanagement/AbstractLifetimeImplementation.h>
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>

//********************************************************************

#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)
NAMESPACE_BEGIN(lifetimemanagement)

//********************************************************************

AbstractLifetimeImplementationI::~AbstractLifetimeImplementationI() throw (Exception) 
{
	LifetimeManagerI::removeInstance(name);
}

//********************************************************************

AbstractLifetimeImplementationI::AbstractLifetimeImplementationI(const String& name) throw (Exception)
: name(name)
{
	LifetimeManagerI::addInstance(name, new LifetimeManagerI(name));
}

//********************************************************************

String AbstractLifetimeImplementationI::getName() const
{
	return name;
}

//********************************************************************

LifetimeManager AbstractLifetimeImplementationI::getLifetimeManager() throw (Exception)
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
