// StringWriter.h

//********************************************************************

#ifndef jlj_io_StringWriter_h
#define jlj_io_StringWriter_h

//********************************************************************

#include <jlj/io/AbstractWriter.h>

//********************************************************************

#include <jlj/lang/StringBuffer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio StringWriterI
	: public AbstractWriterI
{
protected:
	~StringWriterI() throw (IOException);
	friend class TypedReference<StringWriterI>;

public:
	StringWriterI();
	StringWriterI(int initialcapacity);

public:
	String toString() const;

	void write(const wchar_t*, int offset, int length) throw (IOException);

private:
	StringBuffer buffer;

};

//********************************************************************

StrongReferenceCommonDeclaration(StringWriter, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
