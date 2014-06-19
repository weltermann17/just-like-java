// AtomicBoolean.h

//********************************************************************

#ifndef jlj_util_concurrent_atomic_AtomicBoolean_h
#define jlj_util_concurrent_atomic_AtomicBoolean_h

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

class ExportedByJLJutil AtomicBooleanI
	: public ObjectI
{
protected:
	~AtomicBooleanI();
	friend class TypedReference<AtomicBooleanI>;

public:
	AtomicBooleanI(bool value);

	bool compareAndSet(bool expect, bool update);
	bool get() const;
	bool getAndSet(bool newvalue);
	void lazySet(bool newvalue);
	void set(bool newvalue);
	String toString() const;

private:
	int value;

};

//********************************************************************

struct ExportedByJLJutil AtomicBoolean : StrongReference<AtomicBooleanI> 
{
	StrongReferenceCommonDerivateImplementation(AtomicBoolean);

	// some exceptional helpers for more convenience
	AtomicBoolean(bool);
	AtomicBoolean& operator=(bool);
	operator bool() const;
};

//********************************************************************

ExportedByJLJutil bool operator!(const AtomicBoolean&);

//********************************************************************

NAMESPACE_END(atomic)
NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
