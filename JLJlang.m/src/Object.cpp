// Object.cpp

//********************************************************************

#ifndef jlj_lang_Object_cpp
#define jlj_lang_Object_cpp

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
#include <jlj/lang/String.h>
#include <jlj/lang/StringBuffer.h>
#include <jlj/lang/UTF8String.h>

//********************************************************************

#include <jlj/core/ptypes/ptypes.h>

//********************************************************************

#include <stdio.h>
#include <typeinfo>

//********************************************************************
//********************************************************************

#ifndef jlj_lang_Object_i_h
#define jlj_lang_Object_i_h

#ifdef WIN32
#include <jlj/lang/Class.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

template< typename T > 
Class ObjectI::getClass(const T& derived) const
{
	return makeClass(typeid(derived).name());
}	

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

#endif

//********************************************************************

#endif

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

ObjectI::~ObjectI() 
{}

//********************************************************************

ObjectI::ObjectI() 
{}   

//********************************************************************

int ObjectI::hashCode() const
{
#if defined(PLATEFORME_DS64)
	pt::large hash = (pt::large) this;
	StringBuffer buf;
	buf->append(hash);
	return buf->hashCode();
#else
	return (int) this;
#endif
}

//********************************************************************

String ObjectI::toString() const
{
	char buffer[100];
	sprintf(buffer, "0x%08X", this->hashCode());
	return getClass()->getName() + L"@" + String(buffer, strlen(buffer));
}

//********************************************************************

Class ObjectI::getClass() const
{
	return getClass(*this);
}

//********************************************************************

Object ObjectI::clone() const
{
	return this;
}

//********************************************************************

bool ObjectI::equals(const Object& object) const
{
	return this->hashCode() == object->hashCode();
}

//********************************************************************

Class ObjectI::makeClass(const char* name)
{
	return new ClassI(ClassI::normalizeClassName(name));
}

//********************************************************************

String ObjectI::computeClassCastMessage(const char* from, 
										const char* to)
{
	StringBuffer msg;
	msg->append(L"Cannot downcast from <");
	msg->append(String(from, strlen(from)));
	msg->append(L"> to <");
	msg->append(String(to, strlen(to)));
	msg->append(L">.");
	return String(msg); // consume
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
