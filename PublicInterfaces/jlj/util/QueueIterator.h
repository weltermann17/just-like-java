// ArrayListIteratorI.h

//********************************************************************

#ifndef QueueIteratorI_h
#define QueueIteratorI_h

//********************************************************************

#include <jlj/util/Iterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

NAMESPACE_BEGIN(concurrent)
template< typename T > class LinkedBlockingQueue;
NAMESPACE_END(concurrent)

//********************************************************************

template< typename T >
class QueueIteratorI 
	: public jlj::util::IteratorI<T>
{
protected:
	~QueueIteratorI();
	friend class TypedReference<QueueIteratorI<T> >;

public:
	QueueIteratorI(jlj::util::concurrent::LinkedBlockingQueue<T>);

public:	
	bool hasNext();
	T& next();
	void remove();
	
private:
	
	jlj::util::concurrent::LinkedBlockingQueue<T> list;
	T* pt;
	bool canRemove;
};

//********************************************************************

template< typename T >
struct QueueIterator : StrongReference<QueueIteratorI<T> >
{
	StrongReferenceTemplateDerivateImplementation(QueueIterator, T);
};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/QueueIterator.i.h>

//********************************************************************

#endif

// eof
