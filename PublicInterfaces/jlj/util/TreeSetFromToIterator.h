// TreeSetFromToIterator.h

//********************************************************************

#ifndef jlj_util_TreeSetFromToIterator_h
#define jlj_util_TreeSetFromToIterator_h

//********************************************************************

#include <jlj/util/TreeSetIterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class TreeSetFromToIteratorI 
	: public TreeSetIteratorI<T>
{
public:
	~TreeSetFromToIteratorI();

	TreeSetFromToIteratorI(
		const TreeSetBase<T>&, 
		const T& from, 
		const T& to,
		bool hasfrom,
		bool hasto,
		bool includeto);

public:	
	bool hasNext();

private:
	T from;
	T to;
	bool hasfrom;
	bool hasto;
	bool includeto;

};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/TreeSetFromToIterator.i.h>

//********************************************************************

#endif

// eof
