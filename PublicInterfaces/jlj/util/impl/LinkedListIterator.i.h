// LinkedListIterator.i.h

//********************************************************************

#ifndef jlj_util_impl_LinkedListIterator_i_h
#define jlj_util_impl_LinkedListIterator_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > 
LinkedListIteratorI<T>::~LinkedListIteratorI() 
{}

//********************************************************************

template< typename T > 
LinkedListIteratorI<T>::LinkedListIteratorI(LinkedListNode<T>* current, 
											LinkedList<T> list)
											: current(current)
											, list(list)
											, index(0)
											, canAdd(true)
											, canRemove(false)
											, canSet(false)
											, afterNext(false)
{}

//********************************************************************

template< typename T > 
void LinkedListIteratorI<T>::add(const T& element) 
{
	if (canAdd)
	{
		ASSERT(0 != list);
		list->add(index - 1, element);
		canAdd = canRemove = canSet = false;
	}
}

//********************************************************************

template< typename T > 
bool LinkedListIteratorI<T>::hasNext() 
{
	canRemove = canSet = false;
	return 0 != current;
}

//********************************************************************

template< typename T > 
bool LinkedListIteratorI<T>::hasPrevious() 
{
	canRemove = canSet = false;
	return 0 != current;
}

//********************************************************************

template< typename T > 
T& LinkedListIteratorI<T>::next() 
{
	ASSERT(hasNext());
	canAdd = canRemove = canSet = true;
	afterNext = true;
	T& data = current->data;
	current = current->next;
	++index;
	return data;
}

//********************************************************************

template< typename T > 
int LinkedListIteratorI<T>::nextIndex()  
{
	ASSERT(0 != list);
	if (hasNext())
	{
		int i = 0;
		LinkedListNode<T>* node = list->head;
		while (current != node)
		{
			node = node->next;
			++i;
		}
		return i;
	}
	else
	{
		return list->size();
	}
}

//********************************************************************

template< typename T > 
T& LinkedListIteratorI<T>::previous() 
{
	ASSERT(hasPrevious());
	canAdd = canRemove = canSet = true;
	afterNext = false;
	T& data = current->data;
	current = current->prev;
	--index;
	return data;
}

//********************************************************************

template< typename T > 
int LinkedListIteratorI<T>::previousIndex() 
{
	ASSERT(0 != list);
	if (hasPrevious())
	{
		int i = list->size() - 1;
		LinkedListNode<T>* node = list->tail;
		while (current != node)
		{
			node = node->prev;
			--i;
		}
		return i;
	}
	else
	{
		return -1;
	}
}

//********************************************************************

template< typename T > 
void LinkedListIteratorI<T>::remove() 
{
	if (canRemove) 
	{
		LinkedListNode<T>* node;

		if (0 != current)
		{
			node = afterNext ? current->prev : current->next;
		}
		else
		{
			node = afterNext ? list->tail : list->head;
		}

		if (0 != node->next)
		{
			node->next->prev = node->prev;
		}
		if (0 != node->prev)
		{
			node->prev->next = node->next;
		}
		if (list->head == node)
		{
			list->head = node->next;
		}
		if (list->tail == node)
		{
			list->tail = node->prev;
		}

		delete node;
		--(list->sentinel);		
		canAdd = canRemove = canSet = false;
	}
}

//********************************************************************

template< typename T > 
void LinkedListIteratorI<T>::set(const T& element) 
{
	if (canSet)
	{
		current->data = element;
		canSet = false;
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
