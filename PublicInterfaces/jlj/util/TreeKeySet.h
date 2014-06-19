// TreeKeySet.h

//********************************************************************

#ifndef jlj_util_TreeKeySet_h
#define jlj_util_TreeKeySet_h

//********************************************************************

#include <jlj/util/MapEntry.h>
#include <jlj/util/TreeSetBase.h>
#include <jlj/util/TreeSet.h>
#include <jlj/util/TreeKeySetIterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
class TreeKeySetI 
	: public TreeSetBaseI<K>
{
protected:
	~TreeKeySetI();
	friend class TypedReference<TreeKeySetI<K, V> >;

public:
	TreeKeySetI(const TreeSetBase<MapEntry<K, V> >&);	

public:
	bool add(const K&);
	void clear();
	Comparator<K> comparator() const;
	bool contains(const K&) const;
	Iterator<K> iterator();
	Iterator<K> iterator() const;
	bool remove(const K&);
	int size() const; 

public:
	const K& first() const;
	SortedSet<K> headSet(const K& to) throw (IndexOutOfBoundsException);
	const K& last() const;
	SortedSet<K> subSet(const K& from, const K& to) throw (IndexOutOfBoundsException);
	SortedSet<K> tailSet(const K& from) throw (IndexOutOfBoundsException);

	K& lookup(const K&) const;
	TreeNode<K>* lookupNode(const K&) const;
	TreeNode<K>* getRoot() const;

private:
	TreeSetBase<MapEntry<K, V> > backup; 

};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/TreeKeySet.i.h>

//********************************************************************

#endif

// eof
