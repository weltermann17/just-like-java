// ListAnything.cpp

//********************************************************************

#ifndef ListAnything_cpp
#define ListAnything_cpp

//********************************************************************

#include <jlj/util/ListAnything.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/ArrayList.h>
#include <jlj/io/StringWriter.h>
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

//********************************************************************

#include <wchar.h>
#include <string.h>
#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

ListAnythingI::~ListAnythingI() 
{}

//********************************************************************

ListAnythingI::ListAnythingI()
: list(new LinkedListI<Anything>()) 
{}

//********************************************************************

ListAnythingI::ListAnythingI(int capacity) 
: list(new ArrayListI<Anything>(capacity)) 
{}

//********************************************************************

ListAnythingI::ListAnythingI(const List<Anything>& copy)
: list(copy) 
{}

//********************************************************************

ListAnythingI::ListAnythingI(InputStream& in, 
							 Map<int, void*>& references) throw (IOException)
{
	int putback = sizeof(int);
	int tellg = in->tell() - putback;
	int size;
	in->readComplete((char*)&size, 0, sizeof(int));
	size = bigendian(size);
	validate(size);		
	list = new ArrayListI<Anything>(size);
	references->put(tellg, (void*) this);
	Anything last;
	for (int i = 0; i < size; ++i)
	{
		Anything element =  Anything::deserialize__(in, references);
		if (references->containsKey(-1))
		{
			int duplicates = element;
			for (int j = 0; j < duplicates; ++j)
				list->add(last);
			i += duplicates - 1;
			references->remove(-1);
		}
		else
		{
			list->add(element);
			last = element;
		}
	}
}

//********************************************************************

int ListAnythingI::type() const 
{
	return Anything::ANY_LIST;
}

//********************************************************************

void ListAnythingI::serialize__(OutputStream& os,
								Map<void*, int>& references,
								Anything keyreferences) const throw (IOException) 
{
	if (serializeReference(os, references, (void*) this))
	{
		int type = bigendian(Anything::ANY_LIST);
		int l = bigendian(size());
		os->write((char*)&type, sizeof(int));
		os->write((char*)&l, sizeof(int));

		int duplicates = 0;
		AnythingI* last = 0;
		Iterator<Anything> i = list->iterator();
		while (i->hasNext())
		{
			Anything current = i->next();
			if (last == current.get())
				duplicates++;
			else
			{
				if (0 < duplicates)
				{
					int type = bigendian(Anything::ANY_DUPLICATE);
					int dupl = bigendian(duplicates);
					os->write((char*)&type, sizeof(int));
					os->write((char*)&dupl, sizeof(int));
					duplicates = 0;
				}
				current->serialize__(os, references, keyreferences);
				last = current.get();
			}
		}
		if (0 < duplicates)
		{
			int type = bigendian(Anything::ANY_DUPLICATE);
			int dupl = bigendian(duplicates);
			os->write((char*)&type, sizeof(int));
			os->write((char*)&dupl, sizeof(int));
		}
	}
}

//********************************************************************

void ListAnythingI::write__(Writer& writer, 
							int level,
							Map<void*, int>& references) const throw (IOException)
{
	if (this->writeReference(writer, level, references, (void*) this))
	{	
		writer->write(L"\n");
		Iterator<Anything> i = list->iterator();
		int duplicates = 0;
		AnythingI* last = 0;
		bool noelements = true;
		while (i->hasNext())
		{
			if (noelements) 
			{ 
				indent(writer, level + 1);
				writer->write(L"<elements>\n"); 
				noelements = false; 
			}
			Anything current = i->next();
			if (last == current.get())
				duplicates++;
			else
			{
				if (0 < duplicates)
				{
					indent(writer, level + 2);
					writer->write(L"<element>\n");
					indent(writer, level + 3);
					writer->write(L"<anything type=\"Duplicate\" duplicates=\"");
					writer->write(String(duplicates));
					writer->write(L"\" />\n"); 
					duplicates = 0;
					indent(writer, level + 2);
					writer->write(L"</element>\n");
				}
				indent(writer, level + 2);
				writer->write(L"<element>\n");
				current->write__(writer, level + 3, references);
				indent(writer, level + 2);
				writer->write(L"</element>\n");
				last = current.get();
			}			
		}
		if (0 < duplicates)
		{
			indent(writer, level + 2);
			writer->write(L"<element>\n");
			indent(writer, level + 3);
			writer->write(L"<anything type=\"Duplicate\" duplicates=\"");
			writer->write(String(duplicates));
			writer->write(L"\" />\n"); 
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

void ListAnythingI::writeJSON__(UTF8StringBuffer& buffer) const throw (IOException)
{
	buffer->append('[');
	Iterator<Anything> i = list->iterator();
	int c = 0;
	while (i->hasNext())
	{
		if (0 < c) buffer->append(",");
		i->next()->writeJSON__(buffer);
		++c;
	}
	buffer->append(']');
}

//********************************************************************

Object ListAnythingI::clone() const 
{
	Anything c = new ListAnythingI(size());
	Iterator<Anything> i = iterator();
	while (i->hasNext())
	{
		Anything e;
		i->next()->clone()->downcast(e);
		c->add(e);
	}
	return c;
}

//********************************************************************

bool ListAnythingI::equals(const Object& object) const 
{
	ListAnything b;
	if (object->instanceOf(b))
	{
		return list->equals(b->list);
	}
	else
	{
		return false;
	}
}

//********************************************************************

bool ListAnythingI::add(const Anything& element) 
{
	if (this == element.get()) 
	{
		Anything b;
		element->clone()->downcast(b);
		return list->add(b);
	}
	else
	{
		return list->add(element);
	}
}

//********************************************************************

void ListAnythingI::add(int index, const Anything& element) throw (IndexOutOfBoundsException) 
{
	if ((0 > index) || (index > list->size()))
	{
		throw IndexOutOfBoundsException(WITHDETAILS(L"ListAnythingI::add(int index, const Anything& element)"));
	}
	if (this == element.get()) 
	{
		Anything b;
		element->clone()->downcast(b);
		list->add(index, b);
	}
	else
	{
		list->add(index, element);
	}
}

//********************************************************************

void ListAnythingI::clear()
{
	list->clear();
}

//********************************************************************

Anything ListAnythingI::get(int index) const throw (IndexOutOfBoundsException)
{
	return list->get(index);
}

//********************************************************************

Anything ListAnythingI::get(const String& key) const throw (IndexOutOfBoundsException) 
{
	return new NullAnythingI();
}

//********************************************************************

Iterator<Anything> ListAnythingI::iterator()
{
	return list->iterator();
}

//********************************************************************

Iterator<Anything> ListAnythingI::iterator() const
{
	return list->iterator();
}

//********************************************************************

ListIterator<Anything> ListAnythingI::listIterator(bool tail)
{
	return list->listIterator(tail);
}

//********************************************************************

ListIterator<Anything> ListAnythingI::listIterator(bool tail) const
{
	return list->listIterator(tail);
}

//********************************************************************

bool ListAnythingI::remove(const Anything& element) 
{
	return list->remove(element);
}

//********************************************************************

Anything ListAnythingI::remove(const String&) throw (IndexOutOfBoundsException)
{
	return new NullAnythingI();
}

//********************************************************************

Anything ListAnythingI::removeAt(int index) throw (IndexOutOfBoundsException)
{
	return list->removeAt(index);
}

//********************************************************************

int ListAnythingI::size() const
{
	return list->size();
}

//********************************************************************

bool ListAnythingI::addAll(const Collection<Anything>& collection)
{
	return list->addAll(collection);
}

//********************************************************************

bool ListAnythingI::contains(const Anything& value) const
{
	return list->contains(value);
}

//********************************************************************

bool ListAnythingI::containsAll(const Collection<Anything>& collection) const
{
	return list->containsAll(collection);
}

//********************************************************************

bool ListAnythingI::removeAll(const Collection<Anything>& collection)
{
	return list->removeAll(collection);
}

//********************************************************************

bool ListAnythingI::retainAll(const Collection<Anything>& collection)
{
	return list->retainAll(collection);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
