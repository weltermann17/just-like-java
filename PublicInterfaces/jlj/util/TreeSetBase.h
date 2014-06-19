// TreeSetBase.h

//********************************************************************

#ifndef jlj_util_TreeSetBase_h
#define jlj_util_TreeSetBase_h

//********************************************************************

#include <jlj/util/SortedSet.h>
#include <jlj/util/AbstractSet.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > struct TreeNode;

//********************************************************************

template< typename T >
class TreeSetBaseI 
	: public AbstractSetI<T>
	, public SortedSetI<T>
{
public:
	virtual ~TreeSetBaseI() = 0;
	TreeSetBaseI();

public:
	virtual T& lookup(const T&) const = 0;
	virtual TreeNode<T>* lookupNode(const T&) const = 0;
	virtual TreeNode<T>* getRoot() const = 0;

};

//********************************************************************

template< typename T >
struct TreeSetBase : StrongReference<TreeSetBaseI<T> > 
{
	StrongReferenceTemplateDerivateImplementation(TreeSetBase, T);
};

//********************************************************************

template< typename T > 
TreeSetBaseI<T>::~TreeSetBaseI() 
{}

//********************************************************************

template< typename T > 
TreeSetBaseI<T>::TreeSetBaseI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
