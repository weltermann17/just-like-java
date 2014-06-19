// ReferencedObject.h

//********************************************************************

#ifndef jlj_core_memorymanagement_ReferencedObject_h
#define jlj_core_memorymanagement_ReferencedObject_h

//********************************************************************

#include <jlj/core/memorymanagement/Reference.h>
#include <jlj/core/memorymanagement/UnsynchronizedReference.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(memorymanagement)

//********************************************************************

class ExportedByJLJcore ReferencedObjectI
{
protected:
	virtual ~ReferencedObjectI() = 0; 
	friend class TypedReference<ReferencedObjectI>;

	ReferencedObjectI(); 
	
private:
	ReferencedObjectI(const ReferencedObjectI&);
	ReferencedObjectI& operator=(const ReferencedObjectI&);

private:
	friend class GenericReference;
	friend class GenericUnsynchronizedReference;

	void setReferencingBody(void*) const;	
	void* getReferencingBody() const;
	
	mutable void* body;

};
	
//********************************************************************

struct ExportedByJLJcore ReferencedObject : StrongReference<ReferencedObjectI>
{
	StrongReferenceCommonDerivateImplementation(ReferencedObject);
};

//********************************************************************

NAMESPACE_END(memorymanagement)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
