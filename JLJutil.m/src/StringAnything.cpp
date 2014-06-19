// StringAnything.cpp

//********************************************************************

#ifndef StringAnything_cpp
#define StringAnything_cpp

//********************************************************************

#include <jlj/util/StringAnything.h>
#include <jlj/core/platform/bigendian.h>
#include <jlj/core/platform/LocaleGuard.h>
using namespace jlj::core::platform;
#include <jlj/lang/UTF8String.h>
using namespace jlj::lang;

//********************************************************************

#include <wchar.h>
#include <stdio.h>
#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

StringAnythingI::~StringAnythingI() 
{}

//********************************************************************

StringAnythingI::StringAnythingI() 
{}

//********************************************************************

StringAnythingI::StringAnythingI(const String& value)
: value(value) 
{}

//********************************************************************

StringAnythingI::StringAnythingI(String& value)
: value(value) 
{}

//********************************************************************

StringAnythingI::StringAnythingI(InputStream& in, 
								 Map<int, void*>& references) throw (IOException)
{
	char sbuf[8192];
	int putback = sizeof(int);
	int tellg = in->tell() - putback;
	int size;
	in->readComplete((char*)&size, 0, sizeof(int));
	size = bigendian(size);
	validate(size);
	char* s;
	if (size <= 8192) s = sbuf; else s = new char[size];
	in->readComplete(s, 0, size);
	UTF8String utf8value(s, 0, size);
	if (size > 8192) delete [] s;
	value = utf8value;
	references->put(tellg, (void*) this);
}

//********************************************************************

int StringAnythingI::type() const 
{
	return Anything::ANY_STRING;
}

//********************************************************************

StringAnythingI::operator int() const 
{
	int convert;
	return 0 == sscanf(constcharptr(value->toUTF8String()), "%d", &convert) ? 0 : convert;
}

//********************************************************************

StringAnythingI::operator double() const 
{
	LocaleGuard localeguard;
	return atof(constcharptr(value->toUTF8String()));
}

//********************************************************************

StringAnythingI::operator String() const 
{
	return value;
}

//********************************************************************

StringAnythingI::operator UTF8String() const 
{
	return value->toUTF8String();
}

//********************************************************************

void StringAnythingI::serialize__(OutputStream& os,
								  Map<void*, int>& references,
								  Anything) const throw (IOException) 
{
	if (serializeReference(os, references, (void*) this))
	{
		int type = bigendian(Anything::ANY_STRING);
		UTF8String utf8value(value);
		int l = utf8value->length();
		int le = bigendian(l);
		os->write((char*)&type, sizeof(int));
		os->write((char*)&le, sizeof(int));
		os->write(constcharptr(utf8value), l);
	}
}

//********************************************************************

void StringAnythingI::write__(Writer& writer, 
							  int level,
							  Map<void*, int>& references) const throw (IOException)
{
	if (this->writeReference(writer, level, references, (void*) this))
	{
		if (40 < value->length())
		{
			writer->write(L"\n<![CDATA[");
			writer->write(value);
			writer->write(L"]]>\n");
			indent(writer, level);
			writer->write(L"</anything>\n");;
		}
		else
		{
			writer->write(L"<![CDATA[");
			writer->write(value);
			writer->write(L"]]></anything>\n");
		}
	}
}

//********************************************************************

void StringAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	buffer->append('\"');
	StringBuffer copy(value);
	for (int i = 0; i < copy->length(); ++i) 
	{
		wchar_t c = copy->charAt(i);
		if ((L' ' == c) || (L'_' == c) || (L'-' == c) || (L',' == c) || (L'.' == c) || (L':' == c) || (L'0' <= c && c <= L'9') || (L'A' <= c && c <= L'Z') || (L'a' <= c && c <= L'z'))
		{
			char cc = (char) c;
			buffer->append(cc);
		}
		else 
		{
			char buf[16];
			sprintf(buf, "\\u%04x", c);	                  
			buffer->append(buf);
		}
	}
	buffer->append('\"');
}

//********************************************************************

int StringAnythingI::size() const
{
	return value->length();
}

//********************************************************************

Object StringAnythingI::clone() const 
{
	return new StringAnythingI(value);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
