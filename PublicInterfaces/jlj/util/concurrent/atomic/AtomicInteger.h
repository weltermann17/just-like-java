// AtomicInteger.h

//********************************************************************

#ifndef jlj_util_concurrent_atomic_AtomicInteger_h
#define jlj_util_concurrent_atomic_AtomicInteger_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)
NAMESPACE_BEGIN(atomic)

//********************************************************************

class ExportedByJLJutil AtomicIntegerI
	: public ObjectI
{
protected:
	~AtomicIntegerI();
	friend class TypedReference<AtomicIntegerI>;

public:
	AtomicIntegerI(int value);

	int addAndGet(int delta);
	bool compareAndSet(int expect, int update);
	int decrementAndGet();
	int get() const;
	int getAndAdd(int delta);
	int getAndDecrement();
	int getAndIncrement();
	int getAndSet(int newvalue);
	int incrementAndGet();
	void lazySet(int newvalue);
	void set(int newvalue);
	String toString() const;

private:
	int value;

};

//********************************************************************

struct ExportedByJLJutil AtomicInteger : StrongReference<AtomicIntegerI> 
{
	StrongReferenceCommonDerivateImplementation(AtomicInteger);

	// some exceptional helpers for more convenience

	AtomicInteger(int);	
	AtomicInteger& operator=(int);
	int operator++();
	int operator++(int);
	int operator--();
	int operator--(int);
	operator int() const;
};

//********************************************************************

NAMESPACE_END(atomic)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
