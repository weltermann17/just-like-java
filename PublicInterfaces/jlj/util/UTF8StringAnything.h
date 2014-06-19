// UTF8StringAnything.h

//********************************************************************

#ifndef jlj_util_UTF8StringAnything_h
#define jlj_util_UTF8StringAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil UTF8StringAnythingI 
	: public AbstractAnythingI
{
protected:
	~UTF8StringAnythingI();
	friend class TypedReference<UTF8StringAnythingI>;

public:
	UTF8StringAnythingI();
	UTF8StringAnythingI(const UTF8String&);
	UTF8StringAnythingI(UTF8String&); // swap buffer
	UTF8StringAnythingI(InputStream&, Map<int, void*>&) throw (IOException);	

public:
	int type() const;

	operator int() const;
	operator double() const;
	operator String() const;
	operator UTF8String() const;

	Object clone() const;

	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

	int size() const;

private:
	UTF8String value;

};

//********************************************************************

StrongReferenceCommonDeclaration(UTF8StringAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
