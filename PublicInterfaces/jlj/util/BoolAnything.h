// BoolAnything.h

//********************************************************************

#ifndef jlj_util_BoolAnything_h
#define jlj_util_BoolAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil BoolAnythingI 
	: public AbstractAnythingI
{
protected:
	~BoolAnythingI();
	friend class TypedReference<BoolAnythingI>;

public:
	BoolAnythingI();
	BoolAnythingI(bool);
	BoolAnythingI(InputStream&, Map<int, void*>&) throw (IOException);

public:
	int type() const;

	operator bool() const;
	operator int() const;
	int size() const;

	Object clone() const;

	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

private:
	bool value;

};

//********************************************************************

StrongReferenceCommonDeclaration(BoolAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
