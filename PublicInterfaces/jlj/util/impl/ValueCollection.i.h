// ValueCollection.i.h

//********************************************************************

#ifndef jlj_util_ValueCollection_i_h
#define jlj_util_ValueCollection_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V > 
ValueCollectionI<K, V>::~ValueCollectionI() 
{}

//********************************************************************

template< typename K, typename V > 
ValueCollectionI<K, V>::ValueCollectionI(const TreeSetBase<MapEntry<K, V> >& backup_) 
: backup(backup_)
{}

//********************************************************************

template< typename K, typename V > 
bool ValueCollectionI<K, V>::add(const V& element) 
{
	return false;
}

//********************************************************************

template< typename K, typename V > 
void ValueCollectionI<K, V>::clear()
{
	backup->clear();
}

//********************************************************************

template< typename K, typename V > 
Iterator<V> ValueCollectionI<K, V>::iterator()  
{
	return new ValueCollectionIteratorI<K, V>(backup->iterator());
}

//********************************************************************

template< typename K, typename V > 
Iterator<V> ValueCollectionI<K, V>::iterator() const
{
	return new ValueCollectionIteratorI<K, V>(backup->iterator());
}

//********************************************************************

template< typename K, typename V > 
int ValueCollectionI<K, V>::size() const
{
	return backup->size();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
