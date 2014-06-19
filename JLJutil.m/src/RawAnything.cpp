// RawAnything.cpp

//********************************************************************

#ifndef RawAnything_cpp
#define RawAnything_cpp

//********************************************************************

#include <jlj/util/RawAnything.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;
#include <jlj/io/FileWriter.h>
#include <jlj/io/StringWriter.h>
#include <jlj/io/Base64.h>
#include <jlj/util/zip/ZipUtility.h>
using namespace jlj::util::zip;
#include <jlj/io/CRC.h>
using namespace jlj::io;
#include <jlj/io/helper/MemoryMapping.h>
using namespace jlj::io::helper;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

RawAnythingI::~RawAnythingI() 
{
	if (copy || ownership) 
	{
		delete [] value;
	}
}

//********************************************************************

RawAnythingI::RawAnythingI() 
: value(0)
, length(0)
, ownership(false)
, copy(false)
, crc(0)
{}

//********************************************************************

RawAnythingI::RawAnythingI(char* value, 
						   int length, 
						   bool ownership, 
						   bool copy)
						   : value(copy ? new char[length] : value)
						   , length(length)
						   , ownership(ownership)
						   , copy(copy)
						   , crc(0)
{
	if (copy && 0 != value) 
	{
		memcpy(this->value, value, length);
	}
}

//********************************************************************

RawAnythingI::RawAnythingI(InputStream& in, 
						   Map<int, void*>& references) throw (IOException)
						   : value(0)
						   , length(0)
						   , ownership(true)
						   , copy(true) 
						   , crc(0)
{
	int putback = sizeof(int);
	int tellg = in->tell() - putback;
	in->readComplete((char*)&length, 0, sizeof(int));
	length = bigendian(length);
	validate(length);
	value = new char[length];
	in->readComplete(value, 0, length);
	references->put(tellg, (void*) this);
}

//********************************************************************

int RawAnythingI::type() const 
{
	return Anything::ANY_RAW;
}

//********************************************************************

const char* RawAnythingI::buffer() const 
{
	return value;
}

//********************************************************************

void RawAnythingI::serialize__(OutputStream& os,
							   Map<void*, int>& references,
							   Anything) const throw (IOException)
{
	if (serializeReference(os, references, (void*) this))
	{
		int type = bigendian(Anything::ANY_RAW);
		int l = bigendian(length);
		os->write((char*)&type, sizeof(int));
		os->write((char*)&l, sizeof(int));
		os->write(value, length);
	}
}

//********************************************************************

void RawAnythingI::write__(Writer& writer, 
						   int level,
						   Map<void*, int>& references) const throw (IOException)
{
	if (this->writeReference(writer, level, references, (void*) this))
	{
		UTF8String encoded;
		Base64().encode(encoded, value, length);

		writer->write(L"\n<![CDATA[\n");
		FileWriter filewriter;
		if (writer->instanceOf(filewriter))
		{
			filewriter->write(encoded);
		}
		else
		{
			writer->write(encoded);
		}
		writer->write(L"\n]]>\n");
		indent(writer, level);
		writer->write(L"</anything>\n");
	}
}

//********************************************************************

void RawAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	char* zippedbuffer = 0;
	int zippedlength = 0;
	ZipUtility().zipBuffer(zippedbuffer, zippedlength, value, length);
	Base64 base64;
	base64.setLineSeparator("");
	UTF8String encoded;
	base64.encode(encoded, zippedbuffer, zippedlength);
	delete [] zippedbuffer;

	buffer->append("{\"encoded\":\"base64\",\"decodedAndUncompressedSize\":");
	buffer->append(length);
	buffer->append(",\"decodedAndCompressedSize\":");
	buffer->append(zippedlength);
	buffer->append(",\"compression\":\"zip\",\"content\":\"");
	buffer->append(encoded);
	buffer->append("\"}");
}

//********************************************************************

int RawAnythingI::hashCode() const 
{
	if (0 == crc)
	{
		unsigned int dummy = 0;
		crc = CRC().compute(value, length, dummy);	
	}
	return (int) crc;
}

//********************************************************************

bool RawAnythingI::equals(const Object& object) const 
{
	RawAnything b;
	if (object->instanceOf(b))
	{
		return this->length == b->length && 0 == memcmp(this->value, b->value, length);
	}
	else
	{
		return false;
	}
}

//********************************************************************

Object RawAnythingI::clone() const 
{
	return new RawAnythingI(value, length, true, true);
}

//********************************************************************

int RawAnythingI::size() const
{
	return length;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
