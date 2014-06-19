// UTF8StringAnything.cpp

//********************************************************************

#ifndef UTF8StringAnything_cpp
#define UTF8StringAnything_cpp

//********************************************************************

#include <jlj/util/UTF8StringAnything.h>
#include <jlj/core/platform/bigendian.h>
#include <jlj/core/platform/LocaleGuard.h>
using namespace jlj::core::platform;
#include <jlj/lang/UTF8String.h>
#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/io/FileWriter.h>
using namespace jlj::io;
#include <jlj/util/logging/Logger.h>
using namespace jlj::lang;
using namespace jlj::util::logging;

//********************************************************************

#include <wchar.h>
#include <stdio.h>
#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

UTF8StringAnythingI::~UTF8StringAnythingI() 
{}

//********************************************************************

UTF8StringAnythingI::UTF8StringAnythingI() 
{}

//********************************************************************

UTF8StringAnythingI::UTF8StringAnythingI(const UTF8String& value)
: value(value) 
{}

//********************************************************************

UTF8StringAnythingI::UTF8StringAnythingI(UTF8String& value)
: value(value) 
{}

//********************************************************************

UTF8StringAnythingI::UTF8StringAnythingI(InputStream& in, 
										 Map<int, void*>& references) throw (IOException)
{
	int putback = sizeof(int);
	int tellg = in->tell() - putback;
	int size;
	in->readComplete((char*)&size, 0, sizeof(int));
	size = bigendian(size);
	validate(size);
	std::string buf;
	buf.reserve(size + 1);
	buf.resize(size);
	in->readComplete((char*) buf.c_str(), 0, size); // bug fixed, was : size - 1
	buf[size] = 0;
	value = buf;
	references->put(tellg, (void*) this);
}

//********************************************************************

int UTF8StringAnythingI::type() const 
{
	return Anything::ANY_UTF8STRING;
}

//********************************************************************

UTF8StringAnythingI::operator int() const 
{
	int convert;
	return 0 == sscanf(constcharptr(value), "%d", &convert) ? 0 : convert;
}

//********************************************************************

UTF8StringAnythingI::operator double() const 
{
	LocaleGuard localeguard;
	return atof(constcharptr(value));
}

//********************************************************************

UTF8StringAnythingI::operator String() const 
{
	return value->toString();
}

//********************************************************************

UTF8StringAnythingI::operator UTF8String() const 
{
	return value;
}

//********************************************************************

void UTF8StringAnythingI::serialize__(OutputStream& os,
									  Map<void*, int>& references,
									  Anything) const throw (IOException) 
{
	if (serializeReference(os, references, (void*) this))
	{
		int type = bigendian(Anything::ANY_UTF8STRING);
		int l = value->length();
		int le = bigendian(l);
		os->write((char*)&type, sizeof(int));
		os->write((char*)&le, sizeof(int));
		os->write(constcharptr(value), l);
	}
}

//********************************************************************

void UTF8StringAnythingI::write__(Writer& writer, 
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
			FileWriter filewriter;
			if (writer->instanceOf(filewriter))
			{
				filewriter->write(value);
			}
			else
			{
				writer->write(value->toString());
			}
			writer->write(L"]]></anything>\n");
		}
	}
}

//********************************************************************

void UTF8StringAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	buffer->append('\"');
	UTF8StringBuffer copy(value);
	for (int i = 0; i < copy->length(); ++i) 
	{
		char c = copy->charAt(i);
		if ((' ' == c) || ('_' == c) || ('-' == c) || (',' == c) || ('.' == c) || (':' == c) || ('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
		{
			buffer->append(c);
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

int UTF8StringAnythingI::size() const
{
	return value->length();
}

//********************************************************************

Object UTF8StringAnythingI::clone() const 
{
	return new UTF8StringAnythingI(value);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
