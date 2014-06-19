// Date.h

//********************************************************************

#ifndef Date_h
#define Date_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
#include <jlj/lang/Comparable.h>
using namespace jlj::lang;
#include <jlj/core/ptypes/ptime.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil DateI
	: public ComparableI
{
protected:
	~DateI();
	friend class TypedReference<DateI>;

public: 
	/**
	* set datetime to -1 to create the "now" date/timestamp
	* else specify the datetime in milliseconds elapsed since 1970.1.1 00:00:00.0000
	*/
	DateI(double datetime);

public:
	bool equals(const Object&) const;
	int hashCode() const;
	
	String toString() const; // this is GMT not local timezone

	Object clone() const;
	
	/**
	* returns the milliseconds elapsed since 1970.1.1 00:00:00.0000
	*/
	virtual double getTime() const;

	/**
	* return datetime in the format 1970.1.1 00:00:00.0000 adjusted to local timezone
	*/
	virtual String toLocaleString() const;

	int compareTo(const Object&) const;

private: 
	pt::datetime dt;
	String convert(pt::datetime) const;
	
};

//********************************************************************

struct ExportedByJLJutil Date : StrongReference<DateI>
{    
	StrongReferenceCommonDerivateImplementation(Date);

	static Date NOW(); // return the current datetime
};

//********************************************************************

ExportedByJLJutil bool operator<(const Date&, const Date&);

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
