// UnsynchronizedReference.cpp

//********************************************************************

#ifndef UnsynchronizedReference_cpp
#define UnsynchronizedReference_cpp

//********************************************************************

#include <jlj/core/memorymanagement/UnsynchronizedReference.h> 
#include <jlj/core/memorymanagement/ReferencedObject.h> 

//********************************************************************

#include <functional> 

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(memorymanagement)

//********************************************************************

void defaultUnsynchronizedExceptionCallback(const char*)
{
	fprintf(stdout, "GenericUnsynchronizedReference::ExceptionCallback not set properly. This should be done in jlj::lang::Throwable.\n");
	exit(5);
}

//********************************************************************

struct initExceptionCallback
{
	initExceptionCallback()
	{
		GenericUnsynchronizedReference::exceptioncallback = defaultUnsynchronizedExceptionCallback;
	}

} _init;

//********************************************************************
//********************************************************************

class GenericUnsynchronizedReference::Body 
{
public:
	typedef GenericUnsynchronizedReference::DeleteFunction DeleteFunction;

	~Body() {}

	Body(ReferencedObjectI* pointee, DeleteFunction del) 
		: p(pointee), refs(0), strong(0), doDelete(del) {}

	void increment_references()         
	{ 
		++refs;   
	}

	void decrement_references()
	{
		if (0 == --refs)
		{
			doDelete(p);
			delete this;
		}
	}

	void increment_strong_references()  
	{ 
		++strong; 
	}

	void decrement_strong_references() 
	{
		if (0 == --strong)
		{
			doDelete(p);
			p = 0;
		}
	}

	ReferencedObjectI* get() const                   
	{ 
		return p; 
	}

private:
	Body(const Body&) {}
	Body& operator=(const Body&) { return *this; }

	ReferencedObjectI* p;
	int refs;
	int strong;            
	DeleteFunction doDelete; 

};

//********************************************************************
//********************************************************************

GenericUnsynchronizedReference::ExceptionCallback GenericUnsynchronizedReference::exceptioncallback = 0;

//********************************************************************

GenericUnsynchronizedReference::~GenericUnsynchronizedReference() 
{
	body->decrement_references();
}

//********************************************************************

GenericUnsynchronizedReference::GenericUnsynchronizedReference(ReferencedObjectI* pointee, DeleteFunction doDelete)
: body(pointee ? (GenericUnsynchronizedReference::Body*) pointee->getReferencingBody() : 0) 
{ 
	if (0 == body) 
	{
		body = new Body(pointee, doDelete);
		if (pointee) pointee->setReferencingBody(body);
	}
	body->increment_references(); 
}

//********************************************************************

GenericUnsynchronizedReference::GenericUnsynchronizedReference(const GenericUnsynchronizedReference& rhs)
: body(rhs.body) 
{
	body->increment_references(); 
}

//********************************************************************

GenericUnsynchronizedReference& GenericUnsynchronizedReference::operator=(const GenericUnsynchronizedReference& rhs)
{
	if (this != &rhs)
	{
		rhs.body->increment_references();
		body->decrement_references();
		body = rhs.body;
	}
	return *this; 
}

//********************************************************************

void GenericUnsynchronizedReference::swap(GenericUnsynchronizedReference& with) 
{
	Body* tmp = with.body;
	with.body = body;
	body = tmp;
}

//********************************************************************

ReferencedObjectI* GenericUnsynchronizedReference::get() const 
{ 
	return body->get();
}

//********************************************************************

bool GenericUnsynchronizedReference::operator!() const 
{ 
	return 0 == body->get();
}

//********************************************************************

void GenericUnsynchronizedReference::increment_strong_references() const
{
	body->increment_strong_references();
}

//********************************************************************

void GenericUnsynchronizedReference::decrement_strong_references() const
{
	body->decrement_strong_references();
}

//********************************************************************

bool GenericUnsynchronizedReference::less(ReferencedObjectI* rhs) const 
{
	return std::less<void*>()(body->get(), rhs);
}

//********************************************************************
//********************************************************************

/**
*   arglib: A collection of utilities. (E.g. smart pointers).
*   Copyright (C) 1999, 2000 Alan Griffiths.
*
*   This code is part of the 'arglib' library. The latest version of
*   this library is available from:
*
*      http:*www.octopull.demon.co.uk/arglib/
*
*   This code may be used for any purpose provided that:
*
*   1. This complete notice is retained unchanged in any version
*       of this code.
*
*   2. If this code is incorporated into any work that displays
*       copyright notices then the copyright for the 'arglib'
*       library must be included.
*
*   This library is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*   @author alan@octopull.demon.co.uk
**/

//********************************************************************

NAMESPACE_END(memorymanagement)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
