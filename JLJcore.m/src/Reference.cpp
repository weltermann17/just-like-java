// Reference.cpp

//********************************************************************

#ifndef Reference_cpp
#define Reference_cpp

//********************************************************************

#include <jlj/core/memorymanagement/Reference.h> 
#include <jlj/core/memorymanagement/ReferencedObject.h> 

//********************************************************************

#include <jlj/core/ptypes/pasync.h>
#include <functional> 

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(memorymanagement)

//********************************************************************

void defaultExceptionCallback(const char*)
{
	fprintf(stdout, "GenericReference::ExceptionCallback not set properly. This should be done in jlj::lang::Throwable.\n");
	exit(5);
}

//********************************************************************

struct initExceptionCallback
{
	initExceptionCallback()
	{
		GenericReference::exceptioncallback = defaultExceptionCallback;
	}

} _synchronizedinit;

//********************************************************************
//********************************************************************

GenericReference::ExceptionCallback GenericReference::exceptioncallback = 0;

//********************************************************************

#undef WITHMUTEX

//********************************************************************

class GenericReference::Body
{
public:
	typedef GenericReference::DeleteFunction DeleteFunction;

	~Body() 
	{}

	Body(ReferencedObjectI* pointee, DeleteFunction del) 
		: p(pointee), refs(0), strong(0), doDelete(del) 
	{}

	void increment_references()         
	{ 
#ifdef WITHMUTEX
		pt::scopelock scopelock(mutex);
		++refs;
#else
		pt::pincrement(&refs);
#endif
	}

	void decrement_references()
	{
#ifdef WITHMUTEX
		pt::scopelock scopelock(mutex);
		if (0 == --refs) 
		{
			doDelete(p);
			p = 0;
			delete this;			
#else
		if (0 == pt::pdecrement(&refs))
		{
			ReferencedObjectI* ref;
			ref = (ReferencedObjectI*) pt::pexchange((void**) &p, 0);
			doDelete(ref);
			delete this;
#endif
		}
	}

	void increment_strong_references()  
	{ 
#ifdef WITHMUTEX
		pt::scopelock scopelock(mutex);
		++strong;
#else
		pt::pincrement(&strong);
#endif
	}

	void decrement_strong_references() 
	{
#ifdef WITHMUTX
		pt::scopelock scopelock(mutex);
		if (0 == --strong) 
		{
			doDelete(p);
			p = 0;
#else
		if (0 == pt::pdecrement(&strong))
		{
			ReferencedObjectI* ref; 
			ref = (ReferencedObjectI*) pt::pexchange((void**) &p, 0);
			doDelete(ref);
#endif
		}
	}

	ReferencedObjectI* get() const                   
	{ 
#ifdef WITHMUTEX
		pt::scopelock scopelock(mutex);
		return p;
#else
		ReferencedObjectI* ref; 
		pt::pexchange((void**) &ref, p);
		return ref;
#endif
	}

private:
	Body(const Body&) {}
	Body& operator=(const Body&) { return *this; }

	ReferencedObjectI* p;
	int refs;
	int strong;            
	DeleteFunction doDelete; 

#ifdef WITHMUTEX
	mutable pt::mutex mutex;
#endif

};

//********************************************************************
//********************************************************************

GenericReference::~GenericReference() 
{
	body->decrement_references();
}

//********************************************************************

GenericReference::GenericReference(ReferencedObjectI* pointee, DeleteFunction doDelete)
: body(pointee ? (GenericReference::Body*) pointee->getReferencingBody() : 0) 
{ 
	ReferencedObjectI* b; 
	pt::pexchange((void**) &b, (void*) body);
	if (0 == b) 
	{
        Body* nb = new Body(pointee, doDelete);
		pt::pexchange((void**) &body, (void*) nb);
		if (pointee) pointee->setReferencingBody(body);
	}
	body->increment_references(); 
}

//********************************************************************

GenericReference::GenericReference(const GenericReference& rhs)
: body(rhs.body) 
{
	body->increment_references();  
}

//********************************************************************

GenericReference& GenericReference::operator=(const GenericReference& rhs)
{
	if (this != &rhs)
	{
		rhs.body->increment_references();
		body->decrement_references();
		pt::pexchange((void**) &body, (void*) rhs.body);
	}
	return *this; 
}

//********************************************************************

void GenericReference::swap(GenericReference& with) 
{
	Body* tmp = 0;
	pt::pexchange((void**) &tmp, (void*) with.body);
	pt::pexchange((void**) &with.body, (void*) body);
	pt::pexchange((void**) &body, (void*) tmp);
}

//********************************************************************

ReferencedObjectI* GenericReference::get() const 
{ 
	return body->get();
}

//********************************************************************

bool GenericReference::operator!() const 
{ 
	return 0 == body->get();
}

//********************************************************************

void* GenericReference::operator new(size_t)
{ 
	// private, it is not allowed to call ::new on a GenericReference
	return 0;
}

//********************************************************************

void GenericReference::increment_strong_references() const
{
	body->increment_strong_references();
}

//********************************************************************

void GenericReference::decrement_strong_references() const
{
	body->decrement_strong_references();
}

//********************************************************************

bool GenericReference::less(ReferencedObjectI* rhs) const 
{
	return std::less<void*>()(body->get(), rhs);
}

//********************************************************************

NAMESPACE_END(memorymanagement)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
