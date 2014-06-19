// Class.cpp

//********************************************************************

#ifndef Class_cpp
#define Class_cpp				

//********************************************************************

#include <jlj/lang/Class.h> 
#include <jlj/lang/ClassLoader.h> 
#include <jlj/lang/UTF8StringBuffer.h> 

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

ClassI::~ClassI()
{}

//********************************************************************

ClassI::ClassI(const String& name)
: clazzname(name) 
{}

//********************************************************************

ClassI::ClassI(const char* name) 
: clazzname(String(name, strlen(name)))
{}

//********************************************************************

bool ClassI::equals(const Object& object) const 
{
	Class clazz;
	if (object->instanceOf(clazz))
	{
		return getName() == clazz->getName();
	}
	else
		return false;
}

//********************************************************************

int ClassI::hashCode() const
{
	return clazzname->hashCode();
}

//********************************************************************

String ClassI::toString() const
{
	return L"jlj::lang::Class(<" + clazzname + L">)";
}

//********************************************************************

String ClassI::getName() const
{
	return clazzname;
}

//********************************************************************

String ClassI::normalizeClassName(const char* clazzname)
{
	UTF8StringBuffer buf;
	buf->append(clazzname);
	buf->replace("struct ", "");
	buf->replace("class ", "");
	buf->replaceAll(" ", "");
	return buf->toString();
}

//********************************************************************

Object ClassI::newInstance() throw (InstantiationException)
{
	try
	{
		ClassLoader clazzloader = new ClassLoaderI();
		return clazzloader->newInstance(clazzname);
	}
	catch (const InstantiationException&)
	{
		throw;
	}
	catch (const Exception& e)
	{
		throw InstantiationException(e->toString());
	}
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
