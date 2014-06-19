// NullOutputStream.h

//********************************************************************

#ifndef jlj_io_NullOutputStream_h
#define jlj_io_NullOutputStream_h

//********************************************************************

#include <jlj/io/AbstractOutputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio NullOutputStreamI
	: public AbstractOutputStreamI
{
protected:
	~NullOutputStreamI() throw (IOException);
	friend class TypedReference<NullOutputStreamI>;

public:
	NullOutputStreamI();

public:
	void write(const char*, int offset, int length) throw (IOException);

	/**
	* bytes needed after write completed, 
	* use this to compute the size needed to write to a real OutputStream
	*/
	int tell() const throw (IOException); 

private:
	int position;

};

//********************************************************************

StrongReferenceCommonDeclaration(NullOutputStream, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
