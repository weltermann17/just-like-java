// TreeKeySetIterator.h

//********************************************************************

#ifndef jlj_util_TreeKeySetIterator_h
#define jlj_util_TreeKeySetIterator_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
class TreeKeySetIteratorI 
	: public IteratorI<K>
{
public:
	~TreeKeySetIteratorI();
	TreeKeySetIteratorI(const Iterator<MapEntry<K, V> >&);

public:
	bool hasNext();
	K& next();
	void remove();

protected:
	Iterator<MapEntry<K, V> > i;

};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/TreeKeySetIterator.i.h>

//********************************************************************

#endif

// eof
