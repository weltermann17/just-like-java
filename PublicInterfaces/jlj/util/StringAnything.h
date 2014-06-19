// StringAnything.h

//********************************************************************

#ifndef jlj_util_StringAnything_h
#define jlj_util_StringAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil StringAnythingI 
	: public AbstractAnythingI
{
protected:
	~StringAnythingI();
	friend class TypedReference<StringAnythingI>;

public:
	StringAnythingI();
	StringAnythingI(const String&);
	StringAnythingI(String&); // swap buffer
	StringAnythingI(InputStream&, Map<int, void*>&) throw (IOException);	

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
	String value;

};

//********************************************************************

StrongReferenceCommonDeclaration(StringAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
