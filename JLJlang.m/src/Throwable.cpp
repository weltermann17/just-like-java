// Throwable.cpp

//********************************************************************

#ifndef Throwable_cpp
#define Throwable_cpp

//********************************************************************

#include <jlj/lang/Throwable.h> 
#include <jlj/lang/Class.h> 
#include <jlj/lang/StringBuffer.h> 

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

void throwNullPointerException(const char* type)
{
	StringBuffer s;
	s->append(L"operator-> not possible because <");
	s->append(String(type, strlen(type)));
	s->append(L"> is a null pointer");
	throw NullPointerException(String(s));
}

//********************************************************************

struct initNullPointerException
{
	initNullPointerException()
	{
		GenericReference::exceptioncallback = throwNullPointerException;
	}

} _init;

//********************************************************************
//********************************************************************

ThrowableI::~ThrowableI()
{}

//********************************************************************

ThrowableI::ThrowableI()
{}

//********************************************************************

ThrowableI::ThrowableI(const String& message)
: message(message) 
{}

//********************************************************************

bool ThrowableI::equals(const Object& object) const 
{
	Throwable throwable;
	if (object->instanceOf(throwable))
	{
		return getMessage()->equals(throwable->getMessage());
	}
	else
		return false;
}

//********************************************************************

int ThrowableI::hashCode() const
{
	return message->hashCode();
}

//********************************************************************

String ThrowableI::toString() const
{
	StringBuffer msg;
	msg->append(getClass()->getName());
	msg->append(L" : ");
	msg->append(message);
	msg->replace(L"I : ", L" : ");
	return String(msg);
}

//********************************************************************

String ThrowableI::getMessage() const
{
	return message;
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
