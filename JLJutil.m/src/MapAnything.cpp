// MapAnything.cpp

//********************************************************************

#ifndef MapAnything_cpp
#define MapAnything_cpp

//********************************************************************

#include <jlj/util/MapAnything.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/ListAnything.h>

//********************************************************************

#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/TreeMap.h>
#include <jlj/io/StringWriter.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

MapAnythingI::~MapAnythingI() 
{}

//********************************************************************

MapAnythingI::MapAnythingI()
: map(new LinkedHashMapI<String, Anything>())
{}

//********************************************************************

MapAnythingI::MapAnythingI(int initialcapacity, double loadfactor)
: map(new LinkedHashMapI<String, Anything>(initialcapacity, loadfactor))
{}

//********************************************************************

MapAnythingI::MapAnythingI(const Map<String, Anything>& copy)
: map(copy)
{}

//********************************************************************

MapAnythingI::MapAnythingI(InputStream& in, 
						   Map<int, void*>& references) throw (IOException)
{
	int putback = sizeof(int);
	int tellg = in->tell() - putback;
	int size;
	in->readComplete((char*)&size, 0, sizeof(int));
	size = bigendian(size);
	validate(size);
	map = new LinkedHashMapI<String, Anything>((int)(size / .75));
	references->put(tellg, (void*) this);
	for (int i = 0; i < size; ++i)
	{
		Anything key(Anything::deserialize__(in, references));
		map->put(key->toString(), Anything::deserialize__(in, references));
	}
}

//********************************************************************

bool MapAnythingI::equals(const Object& object) const 
{
	MapAnything b;
	if (object->instanceOf(b))
	{
		return map->equals(b->map);
	}
	else
	{
		return false;
	}
}

//********************************************************************

int MapAnythingI::type() const 
{
	return Anything::ANY_MAP;
}

//********************************************************************

void MapAnythingI::serialize__(OutputStream& os,
							   Map<void*, int>& references,
							   Anything keyreferences) const throw (IOException)
{
	if (serializeReference(os, references, (void*) this))
	{
		int type = bigendian(Anything::ANY_MAP);
		int l = bigendian(size());
		os->write((char*)&type, sizeof(int));
		os->write((char*)&l, sizeof(int));
		Iterator<MapEntry<String, Anything> > i = map->entrySet()->iterator();
		while (i->hasNext())
		{
			MapEntry<String, Anything>& current = i->next();
			Anything k = new StringAnythingI(current->key);
			keyreferences->add(k);
			k->serialize__(os, references, keyreferences);
			current->value->serialize__(os, references, keyreferences);
		}
	}
}

//********************************************************************

void MapAnythingI::write__(Writer& writer, 
						   int level,
						   Map<void*, int>& references) const throw (IOException)
{
	if (this->writeReference(writer, level, references, (void*) this))
	{	
		writer->write(L"\n");
		Iterator<MapEntry<String, Anything> > i = map->entrySet()->iterator();
		bool noelements = true;
		while (i->hasNext())
		{
			if (noelements) 
			{ 
				indent(writer, level + 1);
				writer->write(L"<elements>\n"); 
				noelements = false; 
			}
			MapEntry<String, Anything>& e = i->next();
			indent(writer, level + 2);
			writer->write(L"<element>\n");
			indent(writer, level + 3);
			writer->write(L"<key>");
			writer->write(e.getKey());
			writer->write(L"</key>\n");
			indent(writer, level + 3);
			writer->write(L"<value>\n");
			e.getValue()->write__(writer, level + 4, references);
			indent(writer, level + 3);
			writer->write(L"</value>\n");
			indent(writer, level + 2);
			writer->write(L"</element>\n");
		}
		if (!noelements) 
		{
			indent(writer, level + 1);
			writer->write(L"</elements>\n"); 
		}
		indent(writer, level);
		writer->write(L"</anything>\n");
	}
}

//********************************************************************

void MapAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	buffer->append('{');
	Iterator<MapEntry<String, Anything> > i = map->entrySet()->iterator();
	int c = 0;
	while (i->hasNext())
	{
		if (0 < c) buffer->append(",");
		MapEntry<String, Anything> entry = i->next();
		Anything key = new StringAnythingI(entry.key);
		key->writeJSON__(buffer);
		buffer->append(":");
		entry.value->writeJSON__(buffer);
		++c;
	}
	buffer->append('}');
}

//********************************************************************

Object MapAnythingI::clone() const 
{
	Anything c = new MapAnythingI(size(), .75);
	Iterator<MapEntry<String, Anything> > i = entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<String, Anything>& current = i->next();
		Anything v; 
		current->value->clone()->downcast(v);
		c->put(current->key, v);
	}
	return c;
}

//********************************************************************

void MapAnythingI::clear()
{
	map->clear();
}

//********************************************************************

bool MapAnythingI::containsKey(const String& key) const
{
	return map->containsKey(key);
}

//********************************************************************

bool MapAnythingI::containsValue(const Anything& value) const
{
	return map->containsValue(value);
}

//********************************************************************

Set<MapEntry<String, Anything> > MapAnythingI::entrySet() const
{
	return map->entrySet();
}

//********************************************************************

Set<String> MapAnythingI::keySet() const
{
	return map->keySet();
}

//********************************************************************

Anything MapAnythingI::get(int index) const throw (IndexOutOfBoundsException) 
{
	return new NullAnythingI();
}

//********************************************************************

Anything MapAnythingI::get(const String& key) const throw (IndexOutOfBoundsException) 
{
	return map->get(key);
}

//********************************************************************

bool MapAnythingI::put(const String& key, 
					   const Anything& value)
{
	if (this == value.get()) 
	{
		Anything b;
		value->clone()->downcast(b);
		return map->put(key, b);
	}
	else
	{
		return map->put(key, value);
	}
}

//********************************************************************

void MapAnythingI::putAll(const Map<String,Anything>& collection)
{
	map->putAll(collection);
}

//********************************************************************

bool MapAnythingI::remove(const Anything&)
{
	return false;
}

//********************************************************************

Anything MapAnythingI::remove(const String& key) throw (IndexOutOfBoundsException)
{
	return map->remove(key);  
}

//********************************************************************

int MapAnythingI::size() const
{
	return map->size();
}

//********************************************************************

Collection<Anything> MapAnythingI::values() const
{
	return map->values();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
