// String.h

//********************************************************************

#ifndef String_h
#define String_h

//********************************************************************

#include <jlj/lang/UTF8String.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang StringBuffer;

//********************************************************************

class ExportedByJLJlang String
{    
public:
	~String();

	String();    
	String(const std::wstring&);					
    String(const wchar_t*);							
    String(const wchar_t*, int offset, int size);	
    String(const char*, int size);							
    String(const UTF8String&);	
	explicit String(const unsigned short* const, int dummy);

	String(wchar_t);
	String(int);
	String(long);
	String(double);

	explicit String(std::wstring&);	            // swap buffer	
	explicit String(StringBuffer&);	            // swap buffer	
	String& operator=(StringBuffer&);	        // swap buffer	

	bool equals(const String&) const;
	int hashCode() const;
	String toString() const;
	Class getClass() const;
	String clone() const;
	void downcast(String&)const throw (ClassCastException);

	int length() const;

	String toLowerCase() const;
	String toUpperCase() const;
	String trim() const;
	String toMD5() const;

	UTF8String toUTF8String() const;
	operator const wchar_t*() const; 
	operator const std::wstring&() const;

public:
	String(const String&);
	String& operator=(const String&);

	String* operator->();
	const String* operator->() const;

private:
	void* operator new(size_t);
	void init(const char*, int size);

	std::wstring buffer;
	mutable UTF8String utf8string;
	mutable int hash;
	friend class ExportedByJLJlang StringBuffer;	
	friend ExportedByJLJlang const char* constcharptr(const String&);

};

//********************************************************************

ExportedByJLJlang bool operator==(const String&, const String&);
ExportedByJLJlang bool operator==(const wchar_t*, const String&);
ExportedByJLJlang bool operator==(const String&, const wchar_t*);
ExportedByJLJlang bool operator!=(const String&, const String&);
ExportedByJLJlang bool operator!=(const wchar_t*, const String&);
ExportedByJLJlang bool operator!=(const String&, const wchar_t*);
ExportedByJLJlang bool operator<(const String&, const String&);
ExportedByJLJlang bool operator>(const String&, const String&);

//********************************************************************

ExportedByJLJlang String operator+(const String&, const String&);
ExportedByJLJlang String operator+(const wchar_t*, const String&);
ExportedByJLJlang String operator+(const String&, const wchar_t*);

//********************************************************************

ExportedByJLJlang const wchar_t* constwchar_tptr(const String&);
ExportedByJLJlang const char* constcharptr(const String&);

//********************************************************************

ExportedByJLJlang std::ostream& operator<<(std::ostream&, const String&);
ExportedByJLJlang std::wostream& operator<<(std::wostream&, const String&);

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
