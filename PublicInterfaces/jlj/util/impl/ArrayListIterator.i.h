// ArrayListIterator.i.h

//********************************************************************

#ifndef ArrayListIterator_i_h
#define ArrayListIterator_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > 
ArrayListIteratorI<T>::~ArrayListIteratorI() 
{}

//********************************************************************

template< typename T > 
ArrayListIteratorI<T>::ArrayListIteratorI(T* array, bool tail, ArrayList<T> list)
: array(array)
, current(tail ? list->size() : -1)
, list(list)
{
	canAdd = true;
	canRemove = canSet = afterNext = false;
}

//********************************************************************

template< typename T > 
void ArrayListIteratorI<T>::add(const T& element) 
{
	if (canAdd)
	{
		ASSERT(0 != list);
		if (-1 == current)
		{
			current = 0;
		}
		list->add(current++, element);
		canRemove = canSet = false;
	}
}

//********************************************************************

template< typename T > 
bool ArrayListIteratorI<T>::hasNext() 
{
	canRemove = canSet = false;
	int s = list->size();
	if (s - 1 == current) current = s;
	return 0 < s && current < s - 1;
}

//********************************************************************

template< typename T > 
bool ArrayListIteratorI<T>::hasPrevious() 
{
	canRemove = canSet = false;
	if (0 == current) current = -1;
	return 0 < list->size() && current > 0;
}

//********************************************************************

template< typename T > 
T& ArrayListIteratorI<T>::next() 
{
	current++;
	ASSERT( (0 <= current) && (current < list->size()) );
	canAdd = canRemove = canSet = true;
	afterNext = true;
	return array[current];
}

//********************************************************************

template< typename T > 
int ArrayListIteratorI<T>::nextIndex()  
{
	return this->hasNext() ? current + 1 : list->size();
}

//********************************************************************

template< typename T > 
T& ArrayListIteratorI<T>::previous() 
{
	current--;
	ASSERT( (0 <= current) && (current < list->size()) );
	canAdd = canRemove = canSet = true;
	afterNext = false;
	return array[current];
}

//********************************************************************

template< typename T > 
int ArrayListIteratorI<T>::previousIndex() 
{
	return this->hasPrevious() ? current - 1 : -1;
}

//********************************************************************

template< typename T > 
void ArrayListIteratorI<T>::remove() 
{
	if (canRemove)
	{
		ASSERT(0 != list);

		list->removeAt(current);
		if (afterNext) --current;

		canRemove = canSet = false;
	}
}

//********************************************************************

template< typename T > 
void ArrayListIteratorI<T>::set(const T& element) 
{
	if (canSet)
	{
		ASSERT(0 != list);
		list->set(current, element);
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
