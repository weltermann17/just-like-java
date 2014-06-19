// ValueCollectionIterator.h

//********************************************************************

#ifndef jlj_util_ValueCollectionIterator_h
#define jlj_util_ValueCollectionIterator_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
class ValueCollectionIteratorI
	: public IteratorI<V>
{
public:
	~ValueCollectionIteratorI();
	ValueCollectionIteratorI(const Iterator<MapEntry<K, V> >&);

public:
	bool hasNext();
	V& next();
	void remove();

protected:
	Iterator<MapEntry<K, V> > i;

};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/ValueCollectionIterator.i.h>

//********************************************************************

#endif

// eof
