// NullAnything.h

//********************************************************************

#ifndef jlj_util_NullAnything_h
#define jlj_util_NullAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil NullAnythingI 
	: public AbstractAnythingI
{
protected:
	~NullAnythingI();
	friend class TypedReference<NullAnythingI>;

public:
	NullAnythingI(); 

	int type() const;	
	Object clone() const;

	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

};

//********************************************************************

StrongReferenceCommonDeclaration(NullAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
