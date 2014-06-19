// IntAnything.h

//********************************************************************

#ifndef jlj_util_IntAnything_h
#define jlj_util_IntAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil IntAnythingI 
	: public AbstractAnythingI
{
protected:
	~IntAnythingI();
	friend class TypedReference<IntAnythingI>;

public:
	IntAnythingI();
	IntAnythingI(int);
	explicit IntAnythingI(const String&);
	IntAnythingI(InputStream&, Map<int, void*>&) throw (IOException);

public:
	int type() const;

	operator int() const;
	operator UTF8String() const;
	int size() const;

	Object clone() const;

	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

private:
	int value;

};

//********************************************************************

StrongReferenceCommonDeclaration(IntAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
