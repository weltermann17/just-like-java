// TreeSetFromToIterator.i.h

//********************************************************************

#ifndef jlj_util_TreeSetFromToIterator_i_h
#define jlj_util_TreeSetFromToIterator_i_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > class TreeSet;

//********************************************************************

template< typename T > 
TreeSetFromToIteratorI<T>::~TreeSetFromToIteratorI() 
{}

//********************************************************************

template< typename T > 
TreeSetFromToIteratorI<T>::TreeSetFromToIteratorI(const TreeSetBase<T>& s, 
												  const T& from, 
												  const T& to,
												  bool hasfrom,
												  bool hasto,
												  bool includeto)
												  : TreeSetIteratorI<T>(s)
												  , from(from)
												  , to(to)
												  , hasfrom(hasfrom)
												  , hasto(hasto)
												  , includeto(includeto)
{
	if (hasfrom) 
	{
		this->current = s->lookupNode(from);
	}
}

//********************************************************************

template< typename T > 
bool TreeSetFromToIteratorI<T>::hasNext() 
{
	return 0 != current 
		&& ((!hasfrom) || (from < current->data) || (from == current->data) ) 
		&& ((!hasto) || (includeto ? current->data <= to : current->data < to));
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
