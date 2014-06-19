// TreeKeySetIterator.i.h

//********************************************************************

#ifndef jlj_util_TreeKeySetIterator_i_h
#define jlj_util_TreeKeySetIterator_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
TreeKeySetIteratorI<K, V>::~TreeKeySetIteratorI() 
{}

//********************************************************************

template< typename K, typename V >
TreeKeySetIteratorI<K, V>::TreeKeySetIteratorI(const Iterator<MapEntry<K, V> >& i_)
: i(i_) 
{}

//********************************************************************

template< typename K, typename V >
bool TreeKeySetIteratorI<K, V>::hasNext() 
{
	return i->hasNext();
}

//********************************************************************

template< typename K, typename V >
K& TreeKeySetIteratorI<K, V>::next() 
{
	return i->next().key;
}

//********************************************************************

template< typename K, typename V >
void TreeKeySetIteratorI<K, V>::remove() 
{
	i->remove();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
