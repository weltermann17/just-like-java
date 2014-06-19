// TreeSetFromTo.h

//********************************************************************

#ifndef jlj_util_TreeSetFromTo_h
#define jlj_util_TreeSetFromTo_h

//********************************************************************

#include <jlj/util/TreeSet.h>
#include <jlj/util/TreeSetFromToIterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class TreeSetFromToI 
	: public TreeSetBaseI<T>
{
public:
	~TreeSetFromToI();
	TreeSetFromToI(
		const T& from, 
		const T& to, 
		bool hasfrom,
		bool hasto,
		bool includeto,
		const TreeSetBase<T>&);	

public:

public:
	bool add(const T&);
	void clear();
	Comparator<T> comparator() const;
	bool contains(const T&) const;
	Iterator<T> iterator();
	Iterator<T> iterator() const;
	bool remove(const T&);
	int size() const; 

	const T& first() const;
	SortedSet<T> headSet(const T& to) throw (IndexOutOfBoundsException);
	const T& last() const;
	SortedSet<T> subSet(const T& from, const T& to) throw (IndexOutOfBoundsException);
	SortedSet<T> tailSet(const T& from) throw (IndexOutOfBoundsException);

	T& lookup(const T&) const;
	TreeNode<T>* lookupNode(const T&) const;
	TreeNode<T>* getRoot() const;

private:
	bool checkRange(const T&);

	T from;
	T to;
	bool hasfrom;
	bool hasto;
	bool includeto;
	TreeSetBase<T> backup; 

};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/TreeSetFromTo.i.h>

//********************************************************************

#endif

// eof
