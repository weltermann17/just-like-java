// ValueCollectionIterator.i.h

//********************************************************************

#ifndef jlj_util_ValueCollectionIterator_i_h
#define jlj_util_ValueCollectionIterator_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
ValueCollectionIteratorI<K, V>::~ValueCollectionIteratorI() 
{}

//********************************************************************

template< typename K, typename V >
ValueCollectionIteratorI<K, V>::ValueCollectionIteratorI(const Iterator<MapEntry<K, V> >& i_)
: i(i_)
{}

//********************************************************************

template< typename K, typename V >
bool ValueCollectionIteratorI<K, V>::hasNext() 
{
	return i->hasNext();
}

//********************************************************************

template< typename K, typename V >
V& ValueCollectionIteratorI<K, V>::next() 
{
	return i->next().value;
}

//********************************************************************

template< typename K, typename V >
void ValueCollectionIteratorI<K, V>::remove() 
{
	i->remove();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
