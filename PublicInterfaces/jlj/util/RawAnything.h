// RawAnything.h

//********************************************************************

#ifndef jlj_util_RawAnything_h
#define jlj_util_RawAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil RawAnythingI 
	: public AbstractAnythingI
{
protected:
	~RawAnythingI();
	friend class TypedReference<RawAnythingI>;

public:
	RawAnythingI();
	
	/**
	* ownership : 
	* if true the destructor will delete the buffer (either the copy or the given buffer) 
	*
	* copy:
	* if false the constructor takes over the buffer without copying
	* if true the constructor will create a new copy of the buffer
	*/
	RawAnythingI(char*, int length, bool ownership = false, bool copy = true);
	RawAnythingI(InputStream&, Map<int, void*>&) throw (IOException);

public:
	int type() const;
	int hashCode() const;
	bool equals(const Object&) const;
	Object clone() const;

	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

	int size() const;

	const char* buffer() const;

private:
	char* value;
	int length;
	bool ownership;
	bool copy;
	mutable unsigned int crc;

};

//********************************************************************

StrongReferenceCommonDeclaration(RawAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
