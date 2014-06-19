// TestReferencedObject.h

//********************************************************************

#ifndef jlj_core_memorymanagement_TestReferencedObject_h
#define jlj_core_memorymanagement_TestReferencedObject_h

//********************************************************************

#include <jlj/core/memorymanagement/ReferencedObject.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(memorymanagement)

//********************************************************************

struct ExportedByJLJcore TestObject;

//********************************************************************

class ExportedByJLJcore TestObjectI
	: public ReferencedObjectI
{
protected:
	~TestObjectI();
	friend class TypedReference<TestObjectI>;

public:
	TestObjectI(const char* name);

public:
	void setLink(const TestObject&);
	TestObject getLink() const;

private:
	WeakReference<TestObjectI> linkedobject;
	char wastemore[100];
	const char* name;

};

//********************************************************************

StrongReferenceCommonDeclaration(TestObject, ExportedByJLJcore);

//********************************************************************

void testTestObjects();

//********************************************************************

NAMESPACE_END(memorymanagement)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
