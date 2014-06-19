// TestReferencedObject.cpp

//********************************************************************

#ifndef jlj_core_memorymanagement_TestReferencedObject_cpp
#define jlj_core_memorymanagement_TestReferencedObject_cpp

//********************************************************************

#include <jlj/core/memorymanagement/TestReferencedObject.h> 

//********************************************************************

#include <stdio.h>

//********************************************************************
//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(memorymanagement) 

//********************************************************************

TestObjectI::~TestObjectI()
{
	// fprintf(stdout, "TestReferencedObject::~TestReferencedObject() : %s\n", name);
}

//********************************************************************

TestObjectI::TestObjectI(const char* name)
: name(name)
{}   
 
//********************************************************************

void TestObjectI::setLink(const TestObject& testobject)
{
	linkedobject = testobject;
}

//********************************************************************

TestObject TestObjectI::getLink() const
{
	return linkedobject;
}

//********************************************************************
//********************************************************************

void testTestObjects() 
{
	const char* Peter = "Peter";
	const char* Mary = "Mary";
	const char* Paul = "Paul";
	const char* Dad = "Dad";
	
#if 0
	/**
	*  will not compile because it is not allowed to create an ...I class on the stack
	*/
	TestObjectI dad(Dad);

	/**
	*   will not compile because it is not allowed to create a Reference class on the heap
	*/
	TestObject* dad = new TestObject(new TestObjectI(Dad));
#endif

	TestObject dad = new TestObjectI(Dad);

	for (int i = 0; i < 1000000; ++i) 
	{
		TestObject peter = new TestObjectI(Peter);
		dad->setLink(peter);
		{
			TestObject mary = new TestObjectI(Mary);
			mary->setLink(peter);
			{
				TestObject paul = new TestObjectI(Paul);
				paul->setLink(mary);
				peter->setLink(paul);
			}
			peter->setLink(mary);
		}
	}
}

//********************************************************************

NAMESPACE_END(memorymanagement)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
