// StringReader.h

//********************************************************************

#ifndef jlj_io_StringReader_h
#define jlj_io_StringReader_h

//********************************************************************

#include <jlj/io/AbstractReader.h>
#include <jlj/lang/StringBuffer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio StringReaderI
	: public AbstractReaderI
{
protected:
	~StringReaderI() throw (IOException);
	friend class TypedReference<StringReaderI>;

public:
	StringReaderI(const String&);

public:
	void close() throw (IOException);

	void mark(int) throw (IOException);
	bool markSupported() const;
	void reset() throw (IOException);
	bool ready() const throw (IOException);
	int skip(int) throw (IOException);

	int read(String&, int) throw (IOException);
	int read(wchar_t*, int offset, int length) throw (IOException);

private:
	StringBuffer buffer;
	int size;
	int cursor;
	int bookmark;

};

//********************************************************************

StrongReferenceCommonDeclaration(StringReader, ExportedByJLJio);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
