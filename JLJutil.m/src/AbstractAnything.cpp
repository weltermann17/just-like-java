// AbstractAnything.cpp

//********************************************************************

#ifndef AbstractAnything_cpp
#define AbstractAnything_cpp

//********************************************************************

#include <jlj/util/AbstractAnything.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/ListAnything.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/TreeSet.h>
#include <jlj/util/TreeMap.h>

//********************************************************************

#include <jlj/io/NullOutputStream.h>
#include <jlj/io/InMemoryOutputStream.h>
#include <jlj/io/InMemoryInputStream.h>
#include <jlj/io/Base64.h>
#include <jlj/util/zip/ZipUtility.h>
#include <jlj/lang/UTF8StringBuffer.h>
using namespace jlj::io;
using namespace jlj::util::zip;

//********************************************************************

#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

//********************************************************************

#include <wchar.h>
#include <memory.h>
#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

AbstractAnythingI::~AbstractAnythingI() 
{}

//********************************************************************

AbstractAnythingI::AbstractAnythingI() 
{}

//********************************************************************

String AbstractAnythingI::toString() const
{
	return operator String();
}

//********************************************************************

UTF8String AbstractAnythingI::toUTF8String() const
{
	return operator UTF8String();
}

//********************************************************************

UTF8String AbstractAnythingI::toBase64(bool zip) const throw (IOException)
{
	char* buffer;
	try 
	{
		OutputStream nullstream = new NullOutputStreamI();
		serialize(nullstream);
		int len = nullstream->tell();
		UTF8String encoded;
		buffer = new char[len];
		OutputStream outputstream = new InMemoryOutputStreamI(buffer, len);
		serialize(outputstream);
		if (zip) 
		{
			ZipUtility().zipBuffer(buffer, len, buffer, len);
		}
		Base64 base64;
		base64.setLineSeparator("");
		base64.encode(encoded, buffer, len);

		delete [] buffer;
		return encoded;
	} 
	catch (const IOException&) 
	{
		delete [] buffer;
		throw;
	}
}

//********************************************************************

bool AbstractAnythingI::equals(const Object& object) const 
{
	Anything b;

	if (object->instanceOf(b))
	{
		if (type() == b->type())
		{
			switch (type())
			{
			case Anything::ANY_NULL:
				{
					return operator bool() == b->operator bool();
				}
			case Anything::ANY_BOOL:
				{
					return operator bool() == b->operator bool();
				}
			case Anything::ANY_INT:
				{
					return operator int() == b->operator int();
				}
			case Anything::ANY_DOUBLE:
				{
					return operator double() == b->operator double();
				}
			case Anything::ANY_STRING:
				{
					return operator String() == b->operator String();
				}
			case Anything::ANY_UTF8STRING:
				{
					return operator UTF8String() == b->operator UTF8String();
				}
			case Anything::ANY_STREAM:
				{
					return true; // not possible to compare
				}
			default:
				return equals(object);
			}
		}
	}

	return false; 
}

//********************************************************************

int AbstractAnythingI::hashCode() const 
{
	return toString()->hashCode();
}

//********************************************************************

bool AbstractAnythingI::isNull() const 
{
	return Anything::ANY_NULL == type();
}

//********************************************************************

bool AbstractAnythingI::exists() const 
{
	return !isNull();
}

//********************************************************************

void AbstractAnythingI::clear()
{}

//********************************************************************

bool AbstractAnythingI::isEmpty() const
{
	return true;
}

//********************************************************************

bool AbstractAnythingI::remove(const Anything&) 
{
	return false;
}

//********************************************************************

Anything AbstractAnythingI::remove(const String&) throw (IndexOutOfBoundsException)
{
	throw IndexOutOfBoundsException(WITHDETAILS(L"AbstractAnythingI::remove(String)"));
}

//********************************************************************

int AbstractAnythingI::size() const
{
	return 0;
}

//********************************************************************

InputStream AbstractAnythingI::getInputStream() const throw (IOException)
{
	return InputStream();
}

//********************************************************************

AbstractAnythingI::operator bool() const 
{
	return 0 < size();
}

//********************************************************************

AbstractAnythingI::operator int() const 
{
	return size();
}

//********************************************************************

AbstractAnythingI::operator double() const 
{
	return size();
}

//********************************************************************

AbstractAnythingI::operator String() const 
{
	return toUTF8String()->toString();
}

//********************************************************************

AbstractAnythingI::operator UTF8String() const 
{
	return toJSON(64*1024);
}

//********************************************************************

void AbstractAnythingI::add(int, const Anything&) throw (IndexOutOfBoundsException)
{
	throw IndexOutOfBoundsException(WITHDETAILS(L"AbstractAnythingI::add(int, Anything)"));
}

//********************************************************************

void AbstractAnythingI::addAll(int, const Collection<Anything>&) throw (IndexOutOfBoundsException)
{
	throw IndexOutOfBoundsException(WITHDETAILS(L"AbstractAnythingI::addAll(int, Collection<Anything>)"));
}

//********************************************************************

int AbstractAnythingI::indexOf(const Anything&) const
{
	return -1;
}

//********************************************************************

int AbstractAnythingI::lastIndexOf(const Anything&) const
{
	return -1;
}

//********************************************************************

Anything AbstractAnythingI::set(int, const Anything&) throw (IndexOutOfBoundsException)
{ 
	throw IndexOutOfBoundsException(WITHDETAILS(L"AbstractAnythingI::set(int, Anything)"));
}

//********************************************************************

List<Anything> AbstractAnythingI::subList(int, int) const throw (IndexOutOfBoundsException, IllegalArgumentException)
{
	throw IndexOutOfBoundsException(WITHDETAILS(L"AbstractAnythingI::subList(int, int)"));
}

//********************************************************************

bool AbstractAnythingI::containsKey(const String&) const
{
	return false;
}

//********************************************************************

bool AbstractAnythingI::containsValue(const Anything&) const
{
	return false;
}

//********************************************************************

void AbstractAnythingI::putAll(const Map<String,Anything>&)
{
}

//********************************************************************

void AbstractAnythingI::validate(int positiveinteger) throw (IOException)
{
	if (0 > positiveinteger) 
	{
		throw IOException(WITHDETAILS(L"AbstractAnythingI::validate() failed."));
	}
	const int MAXVALIDSIZE = 256*1024*1024;
	if (MAXVALIDSIZE < positiveinteger)
	{
		throw IOException(WITHDETAILS(L"AbstractAnythingI::validate() failed : " + String(positiveinteger) + L" > " + String(MAXVALIDSIZE)));
	}
}

//********************************************************************

void AbstractAnythingI::indent(Writer& writer, int level) const throw (IOException)
{
	for (int i = 0; i < level; ++i) writer->write(L" ");
}

//********************************************************************

void AbstractAnythingI::write(Writer writer) const throw (IOException)
{
	writer->write(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	Map<void*, int> references = new TreeMapI<void*, int>();
	write__(writer, 0, references);
	writer->flush();
}

//********************************************************************

void AbstractAnythingI::serialize(OutputStream os) const throw (IOException)
{
	const char* magiccookie_lo = "ANYT";
	const char* magiccookie_hi = "HING";
	int cookie_lo;
	int cookie_hi;
	memcpy((char*)&cookie_lo, magiccookie_lo, sizeof(int));
	memcpy((char*)&cookie_hi, magiccookie_hi, sizeof(int));
	os->write((char*)&cookie_lo, sizeof(int));
	os->write((char*)&cookie_hi, sizeof(int));

	Map<void*, int> references = new TreeMapI<void*, int>();
	/**
	* need to keep the temporary StringAnythings for all keys on the heap
	* while iterating over map entries,
	* otherwise all keys will reference the first key, which is wrong of course
	*/
	Anything keyreferences = new ListAnythingI();

	serialize__(os, references, keyreferences);
	os->flush();
}

//********************************************************************

bool AbstractAnythingI::writeXml(Writer writer) const throw (IOException)
{
	if (Anything::ANY_MAP != type()) return false;	

	writer->write(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

	Iterator<String> it = keySet()->iterator();
	if (it->hasNext())
	{
		String name = it->next();
		return writeXml__(writer, 0, name, get(name)); 
	}
	else
	{
		return false;
	}
}

//********************************************************************

bool AbstractAnythingI::writeXml__(Writer& writer, 
								   int level,
								   const String& name, 
								   const Anything& list) const throw (IOException)
{
	typedef MapEntry<String, Anything> mapentry;
	const String att = L"#";
	const String val = L"##";

	if (Anything::ANY_LIST != list->type()) return false;

	Iterator<Anything> elements = list->iterator();

	while (elements->hasNext())
	{
		indent(writer, level);
		writer->write(L'<');
		writer->write(name);

		Anything element = elements->next();

		if (Anything::ANY_MAP != element->type()) return false;

		Iterator<mapentry> attributes 
			= element->get(att)->entrySet()->iterator();

		while (attributes->hasNext())
		{
			mapentry& a = attributes->next();
			writer->write(L' ');
			writer->write(a.getKey());
			writer->write(L"=\"");
			writer->write(a.getValue()->toString());
			writer->write(L'\"');
		}

		if (element->get(val)->exists())
		{
			writer->write(L'>');

			String value = element->get(val)->toString();
			if (40 < value->length())
			{
				writer->write(L"\n<![CDATA[");
				writer->write(value);
				writer->write(L"]]>\n");
				indent(writer, level);
				writer->write(L"</");
				writer->write(name);
				writer->write(L">\n");
			}
			else
			{
				writer->write(L"<![CDATA[");
				writer->write(value);
				writer->write(L"]]></");
				writer->write(name);
				writer->write(L">\n");
			}
		}
		else
		{
			bool hasfields = false;
			Iterator<mapentry> fields 
				= element->entrySet()->iterator();

			while (fields->hasNext())
			{
				mapentry& field = fields->next();

				String key = field.getKey();
				if (key->equals(att) || key->equals(val)) continue;

				if (!hasfields) 
				{
					hasfields = true;
					writer->write(L">\n");
				}
				if (!writeXml__(writer, level + 1, key, field.getValue())) 
					return false;
			}

			if (hasfields) 
			{
				indent(writer, level);
				writer->write(L"</");
				writer->write(name);
				writer->write(L">\n");
			}
			else
				writer->write(L"/>\n");
		}
	}
	writer->flush();
	return true;
}

//********************************************************************

bool AbstractAnythingI::serializeReference(OutputStream& os, 
										   Map<void*, int>& references,
										   void* p) const throw (IOException) 
{
	if (references->containsKey(p))
	{
		try
		{
			int type = bigendian(Anything::ANY_REFERENCE);
			int reference = bigendian(references->get(p));
			os->write((char*)&type, sizeof(int));
			os->write((char*)&reference, sizeof(int));
			return false;	
		}
		catch (const Exception& e)
		{
			throw IOException(e->getMessage());
		}
	}
	else
	{
		references->put(p, os->tell());
		return true;	
	}
}

//********************************************************************

bool AbstractAnythingI::writeReference(Writer& writer, 
									   int level,
									   Map<void*, int>& references,
									   void* p) const throw (IOException)
{
	indent(writer, level);
	writer->write(L"<anything type=\""); 
	if (references->containsKey(p))
	{
		try
		{
			int reference = references->get(p);
			writer->write(L"Reference\" ref=\"");
			writer->write(String(reference));
			writer->write(L"\" />\n"); 
			return false;	
		}
		catch (const Exception& e)
		{
			throw IOException(e->getMessage());
		}
	}
	else
	{
		switch (type())
		{
		case Anything::ANY_BOOL:
			writer->write(L"Boolean");
			break;
		case Anything::ANY_INT:
			writer->write(L"Integer");
			break;
		case Anything::ANY_DOUBLE:
			writer->write(L"Double");
			break;
		case Anything::ANY_STRING:
			writer->write(L"String\" size=\"");
			writer->write(String(size()));
			break;
		case Anything::ANY_UTF8STRING:
			writer->write(L"UTF8String\" size=\"");
			writer->write(String(size()));
			break;
		case Anything::ANY_RAW:
			writer->write(L"Raw\" size=\"");
			writer->write(String(size()));
			writer->write(L"\" hashcode=\"");
			writer->write(String(hashCode()));
			break;
		case Anything::ANY_STREAM:
			writer->write(L"Stream\" size=\"");
			writer->write(String(size()));
			break;
		case Anything::ANY_LIST:
			writer->write(L"List\" size=\"");
			writer->write(String(size()));
			break;
		case Anything::ANY_MAP:
			writer->write(L"Map\" size=\"");
			writer->write(String(size()));
			break;
		default:
			writer->write(L"Unknown");
			break;
		}

		int tellp = references->size() + 1;			
		references->put(p, tellp);

		writer->write(L"\" ref=\"");
		writer->write(String(tellp));

		writer->write(L"\">");

		return true;	
	}
}

//********************************************************************

UTF8String AbstractAnythingI::toJSON(int sizehint) const throw (IOException)
{
	UTF8StringBuffer buffer(0 < sizehint ? sizehint : 64*1024);
	writeJSON__(buffer);
	return UTF8String(buffer);
}

//********************************************************************

Anything AbstractAnythingI::get(int index) const  throw (IndexOutOfBoundsException)
{
	throw IndexOutOfBoundsException(WITHDETAILS(L"AbstractAnythingI::get() index = " + String(index)));
}

//********************************************************************

Iterator<Anything> AbstractAnythingI::iterator()
{
	List<Anything> list = new LinkedListI<Anything>();
	return list->iterator();
}

//********************************************************************

Iterator<Anything> AbstractAnythingI::iterator() const
{
	List<Anything> list = new LinkedListI<Anything>();
	return list->iterator();
}

//********************************************************************

ListIterator<Anything> AbstractAnythingI::listIterator(bool tail)
{
	List<Anything> list = new LinkedListI<Anything>();
	return list->listIterator(tail);
}

//********************************************************************

ListIterator<Anything> AbstractAnythingI::listIterator(bool tail) const
{
	List<Anything> list = new LinkedListI<Anything>();
	return list->listIterator(tail);
}

//********************************************************************

Anything AbstractAnythingI::removeAt(int index) throw (IndexOutOfBoundsException)
{
	throw IndexOutOfBoundsException(WITHDETAILS(L"AbstractAnythingI::removeAt(int)"));
}

//********************************************************************

Set<MapEntry<String, Anything> > AbstractAnythingI::entrySet() const
{
	return new TreeSetI<MapEntry<String, Anything> >();
}

//********************************************************************

Set<String> AbstractAnythingI::keySet() const
{
	return new TreeSetI<String>();	
}

//********************************************************************

Collection<Anything> AbstractAnythingI::values() const
{
	return new LinkedListI<Anything>();	
}

//********************************************************************

Anything AbstractAnythingI::get(const String& index) const throw (IndexOutOfBoundsException)
{
	throw IndexOutOfBoundsException(WITHDETAILS(L"AbstractAnythingI::get() index = " + index));
}

//********************************************************************

bool AbstractAnythingI::put(const String&, 
							const Anything&)
{
	return false;
}

//********************************************************************

bool AbstractAnythingI::add(const Anything&)
{
	return false;
}

//********************************************************************

bool AbstractAnythingI::addAll(const Collection<Anything>&)
{
	return false;
}

//********************************************************************

bool AbstractAnythingI::contains(const Anything&) const
{
	return false;
}

//********************************************************************

bool AbstractAnythingI::containsAll(const Collection<Anything>&) const
{
	return false;
}

//********************************************************************

bool AbstractAnythingI::removeAll(const Collection<Anything>&)
{
	return false;
}

//********************************************************************

bool AbstractAnythingI::retainAll(const Collection<Anything>&)
{
	return false;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
