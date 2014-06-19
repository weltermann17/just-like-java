// ArrayList.i.h

//********************************************************************

#ifndef ArrayList_i_h
#define ArrayList_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

TEMPLSPEC
ArrayListI<int>::~ArrayListI() 
{
	free(array);
}

//********************************************************************

TEMPLSPEC
ArrayListI<double>::~ArrayListI() 
{
	free(array);
}

//********************************************************************

TEMPLSPEC
ArrayListI<char*>::~ArrayListI() 
{
	free(array);
}

//********************************************************************

TEMPLSPEC
void ArrayListI<int>::init(int capacity_) 
{
	capacity = capacity_;
	array = (int*)malloc(sizeof(int) * capacity);
	sentinel = 0;
}

//********************************************************************

TEMPLSPEC
void ArrayListI<double>::init(int capacity_) 
{
	capacity = capacity_;
	array = (double*)malloc(sizeof(double) * capacity);
	sentinel = 0;
}

//********************************************************************

TEMPLSPEC
void ArrayListI<char*>::init(int capacity_) 
{
	capacity = capacity_;
	array = (char**)malloc(sizeof(char*) * capacity);
	sentinel = 0;
}

//********************************************************************

TEMPLSPEC
void ArrayListI<int>::increaseCapacity(int minimum, int) 
{
	while (capacity <= minimum)
	{
		capacity = (int)(capacity * (GROWTHRATE/100.));
	}
	array = (int*)realloc(array, sizeof(int) * capacity);
	ASSERT(0 != array);
}

//********************************************************************

TEMPLSPEC
void ArrayListI<double>::increaseCapacity(int minimum, int) 
{
	while (capacity <= minimum)
	{
		capacity = (int)(capacity * (GROWTHRATE/100.));
	}
	array = (double*)realloc(array, sizeof(double) * capacity);
	ASSERT(0 != array);
}

//********************************************************************

TEMPLSPEC
void ArrayListI<char*>::increaseCapacity(int minimum, int) 
{
	while (capacity <= minimum)
	{
		capacity = (int)(capacity * (GROWTHRATE/100.));
	}
	array = (char**)realloc(array, sizeof(char*) * capacity);
	ASSERT(0 != array);
}

//********************************************************************

TEMPLSPEC
void ArrayListI<int>::clear()
{
	free(array);
	init(INITIALCAPACITY);
}

//********************************************************************

TEMPLSPEC
void ArrayListI<double>::clear()
{
	free(array);
	init(INITIALCAPACITY);
}

//********************************************************************

TEMPLSPEC
void ArrayListI<char*>::clear()
{
	free(array);
	init(INITIALCAPACITY);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
