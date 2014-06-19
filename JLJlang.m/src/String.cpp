// String.cpp

//********************************************************************

#ifndef String_cpp
#define String_cpp

//********************************************************************

#include <jlj/lang/String.h>
#include <jlj/lang/StringBuffer.h>
#include <jlj/lang/Class.h>
#include <jlj/lang/Throwable.h>
#include <jlj/lang/UTF8String.h>
#include <jlj/core/ptypes/pasync.h>

//********************************************************************

#include <wchar.h>
#include <sstream>
#include <string>
#include <locale>
#include <typeinfo>

//********************************************************************

#if !defined(_WINDOWS_SOURCE)
#include <iconv.h>
#endif

//********************************************************************

#include <jlj/core/platform/LocaleGuard.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

const int MAXSTACK = 8 * 1024;

//********************************************************************
//********************************************************************

String::~String() 
{}

//********************************************************************

String::String() 
: hash(0)
{}

//********************************************************************

String::String(const String& b)
: buffer(b.buffer)
, hash(b.hash)
{}

//********************************************************************

String& String::operator=(const String& b) 
{
	if (this != &b)
	{
		buffer = b.buffer;
		hash = b.hash;
	}
	return *this;
}

//********************************************************************

String* String::operator->() 
{
	return this;
}

//********************************************************************

const String* String::operator->() const
{
	return this;
}

//********************************************************************

String::String(const std::wstring& s)      
: buffer(s)
, hash(0)
{}

//********************************************************************

String::String(std::wstring& s) 
: hash(0)
{
	buffer.swap(s);
}

//********************************************************************

String::String(StringBuffer& s) 
: hash(0)
{
	buffer.swap(s->buffer);
}

//********************************************************************

String& String::operator=(StringBuffer& b) 
{
	buffer.swap(b->buffer);
	hash = 0;
	return *this;
}

//********************************************************************

String::String(const wchar_t* s) 
: buffer(s)
, hash(0)
{}

//********************************************************************

String::String(const unsigned short* const s, int) 
#if 2 == SIZEOFWCHAR_T
: buffer((const wchar_t*) s)
#else
#error "need to implement this with iconv_open("UTF-16", "UTF-8")"
#endif
, hash(0)
{}

//********************************************************************

String::String(const wchar_t* s, int offset, int size) 
: buffer(s, offset, size)
, hash(0)
{}

//********************************************************************

void String::init(const char* s, int srclen) 
{
#if defined(_WINDOWS_SOURCE) 
	int  destlen = MultiByteToWideChar(
		CP_UTF8,
		0,
		s,
		srclen,
		0,
		0);

	if (destlen + 1 < MAXSTACK)
	{
		wchar_t buf[MAXSTACK];
		destlen = MultiByteToWideChar(
			CP_UTF8,
			0,
			s,
			srclen,
			buf,
			MAXSTACK);
		buffer.assign(buf, destlen);
	}
	else
	{
		wchar_t* buf = new wchar_t[destlen + 1];
		destlen = MultiByteToWideChar(
			CP_UTF8,
			0,
			s,
			srclen,
			buf,
			destlen + 1);
		buffer.assign(buf, destlen);
		delete [] buf;
	}
#else
	static iconv_t cd = 0;
	if (0 == cd) 
	{
#if defined(PLATEFORME_DS64)
		cd = iconv_open("UTF-32", "UTF-8");
#else
		cd = iconv_open("UTF-16", "UTF-8");
#endif
	}

	static pt::rwlock* rw = new pt::rwlock();

	size_t i = srclen * sizeof(char);
	size_t o = srclen * sizeof(wchar_t);
	const size_t destlen = o;

	if (destlen + sizeof(wchar_t) < MAXSTACK)
	{
		wchar_t buf[MAXSTACK];
		char* outptr = (char*) buf;
		char* inptr = (char*) s;
		size_t rc;

		{
			pt::scopewrite scope(*rw);
			rc = iconv(
				cd,
				&inptr,
				&i,
				&outptr,
				&o);
		}

		// never called: iconv_close()

		if (0 == rc) buffer.assign(buf, (destlen - o) / sizeof(wchar_t));
	}
	else
	{
		wchar_t* buf = new wchar_t[o + 1];
		char* outptr = (char*) buf;
		char* inptr = (char*) s;
		size_t rc;

		{
			rc = -1;
			try
			{
				pt::scopewrite scope(*rw);
				rc = iconv(
					cd,
					&inptr,
					&i,
					&outptr,
					&o);
			}
			catch (...)
			{
				fprintf(stderr, "jlj::lang::String::init() : Unknown exception.\n");
			}
		}

		// never called: iconv_close()

		if (0 == rc) buffer.assign(buf, (destlen - o) / sizeof(wchar_t));
		delete [] buf;
	}
#endif
}

//********************************************************************

String::String(const char* s, int size) 
: hash(0)
{
	init(s, size);
}

//********************************************************************

String::String(const UTF8String& s)  
: hash(0)
{
	init(constcharptr(s), s->length());
} 

//********************************************************************

String::String(wchar_t value) 
: hash(0)
{
	std::wostringstream s;
	s << value;
	buffer = s.str();
}

//********************************************************************

String::String(int value) 
: hash(0)
{
	std::wostringstream s;
	s << value;
	buffer = s.str();
}

//********************************************************************

String::String(long value) 
:  hash(0)
{
	std::wostringstream s;
	s << value;
	buffer = s.str();
}

//********************************************************************

String::String(double value) 
: hash(0)
{
	LocaleGuard localeguard;
	std::wostringstream s;
	s.precision(15);
	s << value;
	buffer = s.str();
}

//********************************************************************

void* String::operator new(size_t)
{
	return 0;
}

//********************************************************************

int String::hashCode() const 
{
	if (0 == hash) 
	{
		for (int i = 0; i < buffer.length(); ++i) 
		{ 
			hash = ((hash << 19) - hash) + buffer[i]; 
		} 
	}
	return hash;
}

//********************************************************************

bool String::equals(const String& b) const 
{
	return buffer == b->operator const std::wstring&();
}

//********************************************************************

Class String::getClass() const 
{
	return new ClassI(typeid(*this).name());
}

//********************************************************************

String String::clone() const 
{
	return *this;
}

//********************************************************************

void String::downcast(String& other) const throw (ClassCastException)
{
	other = *this;
}

//********************************************************************

int String::length() const 
{
	return buffer.length();
}

//********************************************************************

String::operator const wchar_t*() const
{
	return buffer.c_str(); 
}

//********************************************************************

String::operator const std::wstring&() const
{
	return buffer; 
}

//********************************************************************

UTF8String String::toUTF8String() const
{ 
	if (0 == utf8string.length()) 
	{
		utf8string = UTF8String(buffer.c_str(), buffer.length());
	}
	return utf8string;
}

//********************************************************************

String String::toString() const
{
	return *this; 
}

//********************************************************************

String String::toLowerCase() const
{
	std::wstring copy(buffer);
	for (std::wstring::iterator i = copy.begin(); copy.end() != i; ++i)
	{
		*i = std::tolower(*i, std::locale());
	}
	return String(copy);
}

//********************************************************************

String String::toUpperCase() const
{
	std::wstring copy(buffer);
	for (std::wstring::iterator i = copy.begin(); copy.end() != i; ++i)
	{
		*i = std::toupper(*i, std::locale());
	}
	return String(copy);
}

//********************************************************************

String String::toMD5() const
{
	return toUTF8String()->toMD5();
}

//********************************************************************

String String::trim() const
{
	std::wstring copy(buffer);
	const std::wstring whitespace(L" \t\n");
    size_t startpos = copy.find_first_not_of(whitespace);
    size_t endpos = copy.find_last_not_of(whitespace);

	if (std::wstring::npos == startpos || std::wstring::npos == endpos)
    {
        copy.resize(0);
    }
    else
	{
        copy = copy.substr(startpos, endpos - startpos + 1);
	}
	return String(copy);
}

//********************************************************************

bool operator==(const String& a, const String& b)
{
	return a->equals(b); 
}

//********************************************************************

bool operator==(const wchar_t* a, const String& b)
{
	return 0 == wcscmp(a, constwchar_tptr(b));
}

//********************************************************************

bool operator==(const String& a, const wchar_t* b)
{
	return 0 == wcscmp(constwchar_tptr(a), b);
}

//********************************************************************

bool operator!=(const String& a, const String& b)
{
	return !a->equals(b); 
}

//********************************************************************

bool operator!=(const wchar_t* a, const String& b)
{
	return 0 != wcscmp(a, constwchar_tptr(b));
}

//********************************************************************

bool operator!=(const String& a, const wchar_t* b)
{
	return 0 != wcscmp(constwchar_tptr(a), b);
}

//********************************************************************

bool operator<(const String& a, const String& b)
{
	return a->operator const std::wstring&() < b->operator const std::wstring&();
}

//********************************************************************

bool operator>(const String& a, const String& b)
{
	return a->operator const std::wstring&() > b->operator const std::wstring&();
}

//********************************************************************

String operator+(const String& a, const String& b)
{
	std::wstring result;
	result.reserve(a->length() + b->length() + 1);
	result.append(a->operator const std::wstring&());
	result.append(b->operator const std::wstring&());
	return String(result);
}

//********************************************************************

String operator+(const wchar_t* a, const String& b)
{
	std::wstring result;
	int l = wcslen(a);
	result.reserve(l + b->length() + 1);
	result.append(a, l);
	result.append(b->operator const std::wstring&());
	return String(result);
}

//********************************************************************

String operator+(const String& a, const wchar_t* b)
{
	std::wstring result;
	int l = wcslen(b);
	result.reserve(a->length() + l + 1);
	result.append(a->operator const std::wstring&());
	result.append(b, l);
	return String(result);
}

//********************************************************************

const wchar_t* constwchar_tptr(const String& s)
{
	return s->operator const wchar_t*();
}

//********************************************************************

const char* constcharptr(const String& s)
{
	s->toUTF8String();
	return s->utf8string->operator const char*();
}

//********************************************************************

std::ostream& operator<<(std::ostream& os, const String& s)
{
	return os << s->toUTF8String();
}

//********************************************************************

std::wostream& operator<<(std::wostream& os, const String& s)
{
	return os << constwchar_tptr(s);
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
