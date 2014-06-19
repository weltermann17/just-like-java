// Uuid.h

//********************************************************************

#ifndef Uuid_h
#define Uuid_h

//********************************************************************

#include <jlj/lang/Comparable.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;
#include <jlj/util/concurrent/locks/Lock.h>
using namespace jlj::util::concurrent::locks;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

struct ExportedByJLJutil Uuid;

//********************************************************************

class ExportedByJLJutil UuidI 
	: public ComparableI
{
public:
	~UuidI();	

public:
	bool equals(const Object&) const;
	int hashCode() const;
	String toString() const;
	int compareTo(const Object&) const;

private:
	friend struct ExportedByJLJutil Uuid;
	friend ExportedByJLJutil bool operator<(const Uuid&, const Uuid&);
	UuidI();

	String uuid;

};

//********************************************************************

struct ExportedByJLJutil Uuid : StrongReference<UuidI>
{    
	StrongReferenceCommonDerivateImplementation(Uuid);
	static Uuid randomUuid();
};

//********************************************************************

ExportedByJLJutil bool operator<(const Uuid&, const Uuid&);

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
