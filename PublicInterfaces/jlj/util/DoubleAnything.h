// DoubleAnything.h

//********************************************************************

#ifndef jlj_util_DoubleAnything_h
#define jlj_util_DoubleAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil DoubleAnythingI 
	: public AbstractAnythingI
{
protected:
	~DoubleAnythingI();
	friend class TypedReference<DoubleAnythingI>;

public:
	DoubleAnythingI();
	DoubleAnythingI(double);
	explicit DoubleAnythingI(const String&);
	DoubleAnythingI(InputStream&, Map<int, void*>&) throw (IOException);

public:
	int type() const;

	operator bool() const;
	operator int() const;
	operator double() const;
	operator UTF8String() const;
	int size() const;

	Object clone() const;

	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

private:
	double value;

};

//********************************************************************

StrongReferenceCommonDeclaration(DoubleAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
