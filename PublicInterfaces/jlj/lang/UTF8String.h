// UTF8String.h

//********************************************************************

#ifndef UTF8String_h
#define UTF8String_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <iostream>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang String;
class ExportedByJLJlang UTF8StringBuffer;

//********************************************************************

class ExportedByJLJlang UTF8String
{    
public:
	~UTF8String();

	UTF8String();    
	UTF8String(const std::string&);					
	UTF8String(const char*);						
	UTF8String(const char*, int offset, int size);	
	UTF8String(const wchar_t*, int size);						
	UTF8String(const String&);						

	UTF8String(char);
	UTF8String(int);
	UTF8String(long);
	UTF8String(double);

	explicit UTF8String(std::string&);					// swap buffer
	explicit UTF8String(UTF8String&);				
	explicit UTF8String(UTF8StringBuffer&);				
	UTF8String& operator=(std::string&);           
	UTF8String& operator=(UTF8StringBuffer&);          

	bool equals(const UTF8String&) const;
	int hashCode() const;
	String toString() const;
	Class getClass() const;
	UTF8String clone() const;
	void downcast(UTF8String&) const throw (ClassCastException);
	
	UTF8String toLowerCase() const;
	UTF8String toUpperCase() const;
	UTF8String toMD5() const;
	UTF8String trim() const;

	int length() const;
	int lengthInUTF8Characters() const;

	operator const char*() const; 
	operator const std::string&() const;

public:
	UTF8String(const UTF8String&);
	UTF8String& operator=(const UTF8String&);

	UTF8String* operator->();
	const UTF8String* operator->() const;

private:
	void* operator new(size_t);
	void init(const wchar_t*, int size);

	std::string buffer;
	mutable int hash;
	friend class ExportedByJLJlang UTF8StringBuffer;	

};

//********************************************************************

ExportedByJLJlang bool operator==(const UTF8String&, const UTF8String&);
ExportedByJLJlang bool operator==(const char*, const UTF8String&);
ExportedByJLJlang bool operator==(const UTF8String&, const char*);
ExportedByJLJlang bool operator!=(const UTF8String&, const UTF8String&);
ExportedByJLJlang bool operator!=(const char*, const UTF8String&);
ExportedByJLJlang bool operator!=(const UTF8String&, const char*);
ExportedByJLJlang bool operator<(const UTF8String&, const UTF8String&);
ExportedByJLJlang bool operator>(const UTF8String&, const UTF8String&);

//********************************************************************

ExportedByJLJlang UTF8String operator+(const UTF8String&, const UTF8String&);
ExportedByJLJlang UTF8String operator+(const char*, const UTF8String&);
ExportedByJLJlang UTF8String operator+(const UTF8String&, const char*);

//********************************************************************

ExportedByJLJlang const char* constcharptr(const UTF8String&);

//********************************************************************

ExportedByJLJlang std::ostream& operator<<(std::ostream&, const UTF8String&);
ExportedByJLJlang std::wostream& operator<<(std::wostream&, const UTF8String&);

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
