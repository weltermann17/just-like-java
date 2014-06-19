// ArrayListIterator.i.h

//********************************************************************

#ifndef QueueIteratorI_i_h
#define QueueIteratorI_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > 
QueueIteratorI<T>::~QueueIteratorI() 
{}

//********************************************************************

template< typename T > 
QueueIteratorI<T>::QueueIteratorI(jlj::util::concurrent::LinkedBlockingQueue<T> bqlist)
: canRemove(false)
{
	list = bqlist;
}

//********************************************************************

template< typename T > 
bool QueueIteratorI<T>::hasNext()
{
	canRemove = false;
	int s = list->size();

	if (s == 0) 
	{
		canRemove = true;
		return false;
	}
	else return true;
}

//********************************************************************

template< typename T > 
T& QueueIteratorI<T>::next()
{
	T* pt = new T[1];
	pt[0] = list->take();

	return pt[0];
}

//********************************************************************

template< typename T >
void QueueIteratorI<T>::remove()
{
	if (canRemove)
	{
		list->take();
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
