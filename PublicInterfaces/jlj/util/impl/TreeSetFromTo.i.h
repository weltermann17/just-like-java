// TreeSetFromToI.i.h

//********************************************************************

#ifndef jlj_util_TreeSetFromToI_i_h
#define jlj_util_TreeSetFromToI_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > 
TreeSetFromToI<T>::~TreeSetFromToI() 
{}

//********************************************************************

template< typename T > 
TreeSetFromToI<T>::TreeSetFromToI(const T& from, 
								  const T& to, 
								  bool hasfrom,
								  bool hasto,
								  bool includeto,
								  const TreeSetBase<T>& backup) 
								  : from(from)
								  , to(to)
								  , hasfrom(hasfrom)
								  , hasto(hasto)
								  , includeto(includeto)
								  , backup(backup)
{}

//********************************************************************

template< typename T > 
bool TreeSetFromToI<T>::add(const T& element) 
{
	if (checkRange(element))
		return backup->add(element);
	else
		return false;
}

//********************************************************************

template< typename T > 
void TreeSetFromToI<T>::clear()
{
	backup->removeAll(this);
}

//********************************************************************

template< typename T > 
Comparator<T> TreeSetFromToI<T>::comparator() const
{
	return backup->comparator();
}

//********************************************************************

template< typename T >
bool TreeSetFromToI<T>::contains(const T& element) const 
{
	if (((TreeSetFromToI<T>*)this)->checkRange(element))
		return backup->contains(element);
	else
		return false;
}

//********************************************************************

template< typename T > 
Iterator<T> TreeSetFromToI<T>::iterator()  
{
	return new TreeSetFromToIteratorI<T>(backup, from, to, hasfrom, hasto, includeto);
}

//********************************************************************

template< typename T > 
Iterator<T> TreeSetFromToI<T>::iterator() const
{
	return new TreeSetFromToIteratorI<T>(backup, from, to, hasfrom, hasto, includeto);
}

//********************************************************************

template< typename T > 
bool TreeSetFromToI<T>::remove(const T& element)
{
	if (checkRange(element))
	{
		if (hasfrom && from == element)
		{
			hasfrom = false;
		}
		return backup->remove(element);
	}
	else
		return false;
}

//********************************************************************

template< typename T > 
int TreeSetFromToI<T>::size() const
{
	int count = 0;
	Iterator<T> i = this->iterator();
	while (i->hasNext())
	{
		count++;
		i->next();
	}
	return count;
}

//********************************************************************

template< typename T > 
const T& TreeSetFromToI<T>::first() const
{
	if (hasfrom)
		return from;
	else
		return backup->first();
}

//********************************************************************

template< typename T > 
SortedSet<T> TreeSetFromToI<T>::headSet(const T& to_) throw (IndexOutOfBoundsException) 
{
	if (checkRange(to_))
		return new TreeSetFromToI<T>(from, to_, hasfrom, true, false, backup); 
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename T > 
const T& TreeSetFromToI<T>::last() const
{
	if (hasto)
	{
		TreeNode<T>* p = backup->lookupNode(to);
		if (p)
		{
			p = p->prev();
			if (p) 
				return p->data;
			else
				return *(T*)0;
		}
		else
			return *(T*)0;
	}
	else
		return backup->last();
}

//********************************************************************

template< typename T > 
SortedSet<T> TreeSetFromToI<T>::subSet(const T& from_, const T& to_) throw (IndexOutOfBoundsException) 
{
	if (checkRange(from_) && checkRange(to_))
		return new TreeSetFromToI<T>(from_, to_, true, true, false, backup); 
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename T > 
SortedSet<T> TreeSetFromToI<T>::tailSet(const T& from_) throw (IndexOutOfBoundsException) 
{
	if (checkRange(from_))
		return new TreeSetFromToI<T>(from_, to, true, hasto, true, backup); 
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename T > 
T& TreeSetFromToI<T>::lookup(const T& element) const
{
	return backup->lookup(element);
}

//********************************************************************

template< typename T > 
TreeNode<T>* TreeSetFromToI<T>::lookupNode(const T& element) const
{
	return backup->lookupNode(element);
}

//********************************************************************

template< typename T > 
TreeNode<T>* TreeSetFromToI<T>::getRoot() const
{
	return backup->getRoot();
}

//********************************************************************

template< typename T > 
bool TreeSetFromToI<T>::checkRange(const T& element) 
{
	TreeNode<T>* f;
	TreeNode<T>* t;
	if (hasfrom)
	{
		f = backup->lookupNode(from);
		if (0 == f) hasfrom = false;
	}
	if (hasto)
	{
		t = backup->lookupNode(to);
		if (0 == t) hasto = false;
	}
	return (!hasfrom || (element > f->data) || (element == f->data) 
		&& (!hasto || (includeto ? element <= t->data : element < t->data)));
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
