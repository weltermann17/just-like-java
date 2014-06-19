// TreeSetIterator.i.h

//********************************************************************

#ifndef TreeSetIterator_i_h
#define TreeSetIterator_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > 
TreeSetIteratorI<T>::~TreeSetIteratorI() 
{}

//********************************************************************

template< typename T > 
TreeSetIteratorI<T>::TreeSetIteratorI(const TreeSetBase<T>& s)
: current(s->getRoot())
, set(s)
, canRemove(false)
{
	if (current)
	{
		while (current->left)
			current = current->left;
	}
}

//********************************************************************

template< typename T > 
bool TreeSetIteratorI<T>::hasNext() 
{
	canRemove = false;
	return 0 != current;
}

//********************************************************************

template< typename T > 
T& TreeSetIteratorI<T>::next() 
{
	canRemove = true;
	T& element = current->data;
	current = current->next();
	return element;
}

//********************************************************************

template< typename T > 
void TreeSetIteratorI<T>::remove() 
{
	if (canRemove)
	{
		if (0 == current) 
		{
			current = set->getRoot();
			while (current->right)
				current = current->right;
			const T& element = current->data;
			set->remove(element);
			current = 0;
		}
		else
		{
			if (current->prev())
			{
				const T& element = current->prev()->data;
				set->remove(element);
			}
		}
		canRemove = false;
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
