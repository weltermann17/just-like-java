// ReferencedObject.cpp

//********************************************************************

#ifndef jlj_core_memorymanagement_ReferencedObject_cpp
#define jlj_core_memorymanagement_ReferencedObject_cpp

//********************************************************************

#include <jlj/core/memorymanagement/ReferencedObject.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(memorymanagement)

//********************************************************************

ReferencedObjectI::~ReferencedObjectI()  
{}

//********************************************************************

ReferencedObjectI::ReferencedObjectI() 
: body(0)
{}   

//********************************************************************

ReferencedObjectI::ReferencedObjectI(const ReferencedObjectI&) 
{}

//********************************************************************

ReferencedObjectI& ReferencedObjectI::operator=(const ReferencedObjectI&)
{
	return *this;
}

//********************************************************************

void ReferencedObjectI::setReferencingBody(void* body) const
{
	this->body = body;
}

//********************************************************************

void* ReferencedObjectI::getReferencingBody() const
{
	return body;
}

//********************************************************************

NAMESPACE_END(memorymanagement)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
