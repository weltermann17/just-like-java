// List.h

//********************************************************************

#ifndef jlj_util_List_h
#define jlj_util_List_h

//********************************************************************

#include <jlj/util/Collection.h>
#include <jlj/util/ListIterator.h>

#include <jlj/lang/Throwable.h>
using jlj::lang::IndexOutOfBoundsException;
using jlj::lang::IllegalArgumentException;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > struct List; 

//********************************************************************

template< typename T >
class ListI 
	: public virtual CollectionI<T>
{
public:
	~ListI() = 0;
	ListI();

public:
	bool add(const T&) = 0;
	bool addAll(const Collection<T>&) = 0;

public:
	virtual void add(int index, const T&) throw (IndexOutOfBoundsException) = 0;
	virtual void addAll(int index, const Collection<T>&) throw (IndexOutOfBoundsException) = 0;
	virtual T get(int index) const throw (IndexOutOfBoundsException) = 0;
	virtual int indexOf(const T&) const = 0;
	virtual ListIterator<T> listIterator(bool tail = false) = 0;
	virtual ListIterator<T> listIterator(bool tail = false) const = 0;
	virtual int lastIndexOf(const T&) const = 0;
	virtual T removeAt(int index) throw (IndexOutOfBoundsException) = 0;
	virtual T set(int index, const T&) throw (IndexOutOfBoundsException) = 0;
	virtual List<T> subList(int from, int to) const throw (IndexOutOfBoundsException, IllegalArgumentException) = 0;

};

//********************************************************************

template< typename T >
struct List : StrongReference<ListI<T> >
{
	StrongReferenceTemplateDerivateImplementation(List, T);
};

//********************************************************************

template< typename T > 
ListI<T>::~ListI() 
{}

//********************************************************************

template< typename T > 
ListI<T>::ListI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
