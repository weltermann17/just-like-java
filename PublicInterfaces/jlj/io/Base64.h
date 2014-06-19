// Base64.h

//********************************************************************

#ifndef jlj_io_Base64_h
#define jlj_io_Base64_h

//********************************************************************

#include <jlj/lang/UTF8String.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

class ExportedByJLJio Base64 
{
public:
	~Base64(); // final
	Base64();

public:	
	bool encode(UTF8String&, char*, int);
	bool decode(char*&, int&, const UTF8String&);

	int getLineLength() const;
	void setLineLength(int);

	UTF8String getLineSeparator() const;
	void setLineSeparator(const UTF8String&);

protected:
	enum { PAD64 = -2, IGNORE64 = -1 };

	char* valuetochar;
	int* chartovalue;
	int linelength;
	char lineseparator[80];

private:
	void append(char*, int&, char) const;
	void* operator new(size_t);

};

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
