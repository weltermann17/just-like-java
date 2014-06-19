// InMemoryOutputStream.h

//********************************************************************

#ifndef jlj_io_InMemoryOutputStream_h
#define jlj_io_InMemoryOutputStream_h

//********************************************************************

#include <jlj/io/AbstractOutputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio InMemoryOutputStreamI
	: public AbstractOutputStreamI
{
protected:
	~InMemoryOutputStreamI() throw (IOException);
	friend class TypedReference<InMemoryOutputStreamI>;

public:
	InMemoryOutputStreamI(char* buffer, int buffersize);

public:
	void write(const char*, int offset, int length) throw (IOException);

	int tell() const throw (IOException);

private:
	char* buffer;
	int buffersize;
	int position;

};

//********************************************************************

StrongReferenceCommonDeclaration(InMemoryOutputStream, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
