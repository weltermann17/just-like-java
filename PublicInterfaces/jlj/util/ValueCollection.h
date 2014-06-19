// ValueCollection.h

//********************************************************************

#ifndef ValueCollection_h
#define ValueCollection_h

//********************************************************************

#include <jlj/util/AbstractCollection.h>
#include <jlj/util/TreeSet.h>
#include <jlj/util/MapEntry.h>
#include <jlj/util/ValueCollectionIterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
class ValueCollectionI 
	: public AbstractCollectionI<V>
{
protected:
	~ValueCollectionI();
	friend class TypedReference<ValueCollectionI<K, V> >;

public:
	ValueCollectionI(const TreeSetBase<MapEntry<K, V> >&);	

public:
	bool add(const V&);
	void clear();
	Iterator<V> iterator();
	Iterator<V> iterator() const;
	int size() const; 

private:
	TreeSetBase<MapEntry<K, V> > backup; 

};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/ValueCollection.i.h>

//********************************************************************

#endif

// eof
