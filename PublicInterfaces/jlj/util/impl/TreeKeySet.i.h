// TreeKeySet.i.h

//********************************************************************

#ifndef jlj_util_TreeKeySet_i_h
#define jlj_util_TreeKeySet_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V > 
TreeKeySetI<K, V>::~TreeKeySetI() 
{}

//********************************************************************

template< typename K, typename V > 
TreeKeySetI<K, V>::TreeKeySetI(const TreeSetBase<MapEntry<K, V> >& backup) 
: backup(backup)
{}

//********************************************************************

template< typename K, typename V > 
bool TreeKeySetI<K, V>::add(const K& element) 
{
	return false;
}

//********************************************************************

template< typename K, typename V > 
void TreeKeySetI<K, V>::clear()
{
	backup->clear();
}

//********************************************************************

template< typename K, typename V > 
Comparator<K> TreeKeySetI<K, V>::comparator() const
{
	/*
	* this is not correct, it should return backup->comparator(),
	* but this is a Comparator<MapEntry<K, V> > and not compatible
	*
	* it's ok because this will probably never used
	*/
	return new TreeSetI<K>::lessthancomparator();
}

//********************************************************************

template< typename K, typename V >
bool TreeKeySetI<K, V>::contains(const K& element) const 
{
	return backup->contains(MapEntry<K, V>(element));
}

//********************************************************************

template< typename K, typename V > 
Iterator<K> TreeKeySetI<K, V>::iterator()  
{
	return new TreeKeySetIteratorI<K, V>(backup->iterator());
}

//********************************************************************

template< typename K, typename V > 
Iterator<K> TreeKeySetI<K, V>::iterator() const
{
	return new TreeKeySetIteratorI<K, V>(backup->iterator());
}

//********************************************************************

template< typename K, typename V > 
bool TreeKeySetI<K, V>::remove(const K& element)
{
	return backup->remove(MapEntry<K, V>(element));
}

//********************************************************************

template< typename K, typename V > 
int TreeKeySetI<K, V>::size() const
{
	return backup->size();
}

//********************************************************************

template< typename K, typename V > 
const K& TreeKeySetI<K, V>::first() const
{
	return backup->first().getKey();
}

//********************************************************************

template< typename K, typename V > 
SortedSet<K> TreeKeySetI<K, V>::headSet(const K& to_) throw (IndexOutOfBoundsException)
{
	MapEntry<K, V> f;
	MapEntry<K, V> t = MapEntry<K, V>(to_);
	
	if (backup->contains(t))
	{
		std::cout << "headSet() in TreeKeySetI.i.h has to be checked again because of TreeSetFromTo" << std::endl;
		return new TreeKeySetI<K, V>(
			new TreeSetFromToI<MapEntry<K, V> >(f, t, false, true, false, backup));
	}
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
const K& TreeKeySetI<K, V>::last() const
{
	return backup->last().getKey();
}

//********************************************************************

template< typename K, typename V > 
SortedSet<K> TreeKeySetI<K, V>::subSet(const K& from_, const K& to_) throw (IndexOutOfBoundsException)
{
	MapEntry<K, V> f = MapEntry<K, V>(from_);
	MapEntry<K, V> t = MapEntry<K, V>(to_);
	
	if (backup->contains(f) && backup->contains(t))
	{
		std::cout << "subSet() in TreeKeySetIi.h has to be checked again because of TreeSetFromTo" << std::endl;
		return new TreeKeySetI<K, V>(
			new TreeSetFromToI<MapEntry<K, V> >(f, t, true, true, false,backup));
	}
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
SortedSet<K> TreeKeySetI<K, V>::tailSet(const K& from_) throw (IndexOutOfBoundsException)
{
	MapEntry<K, V> f = MapEntry<K, V>(from_);
	MapEntry<K, V> t;
	
	if (backup->contains(f))
	{
		std::cout << "subSet() in TreeKeySetIi.h has to be checked again because of TreeSetFromTo" << std::endl;

		return new TreeKeySetI<K, V>(
			new TreeSetFromToI<MapEntry<K, V> >(f, t, true, false, false, backup));
	}
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
K& TreeKeySetI<K, V>::lookup(const K& element) const
{
	return backup->lookup(MapEntry<K, V>(element)).key;
}

//********************************************************************

template< typename K, typename V > 
TreeNode<K>* TreeKeySetI<K, V>::lookupNode(const K& element) const
{
	return 0;
}

//********************************************************************

template< typename K, typename V > 
TreeNode<K>* TreeKeySetI<K, V>::getRoot() const
{
	return 0;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
