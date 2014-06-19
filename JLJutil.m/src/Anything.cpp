// Anything.cpp

//********************************************************************

#ifndef Anything_cpp
#define Anything_cpp

//********************************************************************

#include <jlj/util/Anything.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/BoolAnything.h>
#include <jlj/util/IntAnything.h>
#include <jlj/util/DoubleAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/UTF8StringAnything.h>
#include <jlj/util/RawAnything.h>
#include <jlj/util/StreamAnything.h>
#include <jlj/util/ListAnything.h>

#include <jlj/util/MapAnything.h>


//********************************************************************

#include <jlj/util/TreeMap.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/ArrayList.h>

//********************************************************************

#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/io/InMemoryInputStream.h>
#include <jlj/io/Base64.h>
#include <jlj/util/zip/ZipUtility.h>
using namespace jlj::io;
using namespace jlj::util::zip;

//********************************************************************

#include <jlj/util/json/JSON_parser.h>

//********************************************************************

#include <jlj/core/platform/bigendian.h>
#include <jlj/core/platform/LocaleGuard.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

AnythingI::~AnythingI() 
{}

//********************************************************************

AnythingI::AnythingI() 
{}

//********************************************************************
//********************************************************************

Anything::Anything(bool value) 
: StrongReference<AnythingI>(new BoolAnythingI(value))
{}

//********************************************************************

Anything::Anything(int value) 
: StrongReference<AnythingI>(new IntAnythingI(value))
{}

//********************************************************************

Anything::Anything(double value) 
: StrongReference<AnythingI>(new DoubleAnythingI(value))
{}

//********************************************************************

Anything::Anything(const String& value) 
: StrongReference<AnythingI>(new StringAnythingI(value))
{}

//********************************************************************

Anything::Anything(const UTF8String& value) 
: StrongReference<AnythingI>(new UTF8StringAnythingI(value))
{}

//********************************************************************

Anything::Anything(char* value, int length, bool ownership, bool copy) 
: StrongReference<AnythingI>(new RawAnythingI(value, length, ownership, copy))
{}

//********************************************************************

Anything::Anything(InputStream i) 
: StrongReference<AnythingI>(new StreamAnythingI(i))
{}

//********************************************************************

Anything::Anything(List<Anything> copy) 
: StrongReference<AnythingI>(new ListAnythingI(copy))
{}

//********************************************************************

Anything::Anything(Map<String, Anything> copy) 
: StrongReference<AnythingI>(new MapAnythingI(copy))
{}

//********************************************************************

Anything Anything::decodeInputStream(InputStream in) throw (IOException) 
{
	int cookie_lo;
	int cookie_hi;

	int n = in->readComplete((char*)&cookie_lo, 0, sizeof(int));
	if (0 == n) return new NullAnythingI(); 

	in->readComplete((char*)&cookie_hi, 0, sizeof(int));

	bool isAnything = 
		MAGICCOOKIE_LO == bigendian(cookie_lo) && MAGICCOOKIE_HI == bigendian(cookie_hi);

	if (isAnything)
	{
		Map<int, void*> references = new TreeMapI<int, void*>();
		return deserialize__(in, references);
	}
	else
	{
		throw IOException(WITHDETAILS(L"InputStream does not contain an Anything."));	
	}
}

//********************************************************************

Anything Anything::decodeBase64(const UTF8String& base64formattedutf8string) throw (IOException) 
{
	char* buffer = 0;
	try 
	{
		int len = 0;
		Base64().decode(buffer, len, base64formattedutf8string);
		ZipUtility().unzipBuffer(buffer, len, buffer, len);
		Anything result = decodeInputStream(new InMemoryInputStreamI(buffer, len));
		delete [] buffer;
		return result;
	} 
	catch (const IOException&) 
	{
		delete [] buffer;
		throw;
	}
}

//********************************************************************
//********************************************************************

#undef NEED_TO_DEBUG_JSONPARSER
#if defined(NEED_TO_DEBUG_JSONPARSER)

static size_t s_Level = 0;

static const char* s_pIndention = "  ";

static int s_IsKey = 0;

static void print_indention()
{
    size_t i;
    
    for (i = 0; i < s_Level; ++i) {
        printf(s_pIndention);
    }
}
 
#define assert(x)

static int print(void* ctx, int type, const JSON_value* value)
{
    switch(type) {
    case JSON_T_ARRAY_BEGIN:    
        if (!s_IsKey) print_indention();
        s_IsKey = 0;
        printf("[\n");
        ++s_Level;
        break;
    case JSON_T_ARRAY_END:
        assert(!s_IsKey);
        if (s_Level > 0) --s_Level;
        print_indention();
        printf("]\n");
        break;
   case JSON_T_OBJECT_BEGIN:
       if (!s_IsKey) print_indention();
       s_IsKey = 0;
       printf("{\n");
        ++s_Level;
        break;
    case JSON_T_OBJECT_END:
        assert(!s_IsKey);
        if (s_Level > 0) --s_Level;
        print_indention();
        printf("}\n");
        break;
    case JSON_T_INTEGER:
        if (!s_IsKey) print_indention();
        s_IsKey = 0;
        printf("integer: "JSON_PARSER_INTEGER_SPRINTF_TOKEN"\n", value->vu.integer_value);
        break;
    case JSON_T_FLOAT:
        if (!s_IsKey) print_indention();
        s_IsKey = 0;
        printf("float: %s\n", value->vu.str.value); /* We wanted stringified floats */
        break;
    case JSON_T_NULL:
        if (!s_IsKey) print_indention();
        s_IsKey = 0;
        printf("null\n");
        break;
    case JSON_T_TRUE:
        if (!s_IsKey) print_indention();
        s_IsKey = 0;
        printf("true\n");
        break;
    case JSON_T_FALSE:
        if (!s_IsKey) print_indention();
        s_IsKey = 0;
        printf("false\n");
        break;
    case JSON_T_KEY:
        s_IsKey = 1;
        print_indention();
        printf("key = '%s', value = ", value->vu.str.value);
        break;   
    case JSON_T_STRING:
        if (!s_IsKey) print_indention();
        s_IsKey = 0;
        printf("string: '%s'\n", value->vu.str.value);
        break;
    default:
        assert(0);
        break;
    }
    
    return 1;
}

#endif

//********************************************************************

#define push(stack, item) stack->add(item)

#define pop(stack) stack->removeAt(stack->size() - 1)

#define top(stack) stack->get(stack->size() - 1)

#define more(stack) (0 < stack->size())

static int createAnythingCallback(void* ctx, int type, const JSON_value* value) throw (IOException) 
{
	AnythingI* stack = (AnythingI*) ctx;

	// push phase

	switch(type) 
	{
	case JSON_T_ARRAY_BEGIN:   
		push(stack, new ListAnythingI());
		break;
	case JSON_T_ARRAY_END:   
		break;
	case JSON_T_OBJECT_BEGIN:
		push(stack, new MapAnythingI());
		break;
	case JSON_T_OBJECT_END:
		break;
	case JSON_T_INTEGER:
		push(stack, new IntAnythingI(value->vu.integer_value));
		break;
	case JSON_T_FLOAT:
		{
			LocaleGuard localeguard;
			push(stack, new DoubleAnythingI(atof(value->vu.str.value)));
		}
		break;
	case JSON_T_NULL:
		push(stack, new NullAnythingI());
		break;
	case JSON_T_TRUE:
		push(stack, new BoolAnythingI(true));
		break;
	case JSON_T_FALSE:
		push(stack, new BoolAnythingI(false));
		break;
	case JSON_T_KEY:
		push(stack, new StringAnythingI(String(value->vu.str.value, strlen(value->vu.str.value))));
		break;   
	case JSON_T_STRING:
		push(stack, new StringAnythingI(String(value->vu.str.value, strlen(value->vu.str.value))));
		break;
	default:
		throw IOException(WITHDETAILS("Type not handled."));
	}

	// pop phase

	switch(type) 
	{
	case JSON_T_ARRAY_BEGIN:   
	case JSON_T_OBJECT_BEGIN:
	case JSON_T_KEY:
		break;
	case JSON_T_ARRAY_END:
	case JSON_T_OBJECT_END:
	case JSON_T_INTEGER:
	case JSON_T_FLOAT:
	case JSON_T_NULL:
	case JSON_T_TRUE:
	case JSON_T_FALSE:
	case JSON_T_STRING:
		{
			Anything json = pop(stack);
			if (more(stack))
			{
				Anything jsonfather = top(stack);
				switch (jsonfather->type()) 
				{
				case Anything::ANY_LIST:
					jsonfather->add(json);
					break;
				case Anything::ANY_STRING:
					{
						String key = pop(stack)->toString();
						if (more(stack) && Anything::ANY_MAP == top(stack)->type())
						{
							jsonfather = top(stack);
							jsonfather->put(key, json);
						}
						else
						{
							throw IOException(WITHDETAILS("Invalid object on stack, expecting a map."));
						}
						break;
					}
				default:
					throw IOException(WITHDETAILS("Invalid object on stack, expecting a list or a key."));
				}
			}
			else
			{
				switch (json->type()) 
				{
				case Anything::ANY_LIST:
				case Anything::ANY_MAP:
					push(stack, json);
					break;
				default:
					throw IOException(WITHDETAILS("Invalid stack, expecting more input."));
				}
				break;
			}
			break;
		}
	default:
		throw IOException(WITHDETAILS("JSON type not handled."));
	}

	return 1;
}

//********************************************************************

Anything Anything::decodeJSON(const UTF8String& jsonstring, bool errorhandling) throw (IOException) 
{
	char* buffer = 0;
	struct JSON_parser_struct* jc = 0;

	try
	{
		char errbuf[512];

		JSON_config config;
		init_JSON_config(&config);

		Anything stack = new ListAnythingI(); 
		UTF8StringBuffer trace;
		if (errorhandling) trace->ensureCapacity(jsonstring->length());

		config.depth = -1;
		config.callback = &createAnythingCallback;
		config.callback_ctx = (void*) stack.get();
		config.allow_comments = 0;
		config.handle_floats_manually = 1;

		jc = new_JSON_parser(&config);

		int len = jsonstring->length() + 1;
		buffer = new char[len];
		strcpy(buffer, constcharptr(jsonstring));

		for (int i = 0; i < len; ++i) 
		{
			int nextchar = buffer[i];
			if (nextchar <= 0) break;
			if (!JSON_parser_char(jc, nextchar)) 
			{
				if (errorhandling) trace->append(" <-- syntax error\n");
				sprintf(errbuf, "JSON_parser_char: syntax error at position : %d", i);
				trace->append(errbuf);
				throw IOException(WITHDETAILS(String(trace->toString())));
			}
			if (errorhandling) trace->append((char) nextchar);
		}

		if (!JSON_parser_done(jc)) 
		{
			if (errorhandling) trace->append(" <-- syntax error\n");
			sprintf(errbuf, "JSON_parser_end: syntax error, not yet done.");
			trace->append(errbuf);
			throw IOException(WITHDETAILS(String(trace->toString())));
		}

		// cleanup
		delete_JSON_parser(jc);
		delete [] buffer;

		return pop(stack);
	}
	catch (const IOException&)
	{
		delete_JSON_parser(jc);
		delete [] buffer;
		throw;
	}
	catch (const Exception& e)
	{
		delete_JSON_parser(jc);
		delete [] buffer;
		throw IOException(e->toString());
	}
}

//********************************************************************o

Anything Anything::deserialize__(InputStream& in,
								 Map<int, void*>& references) throw (IOException) 
{
	int type;
	in->readComplete((char*)&type, 0, sizeof(int));
	type = bigendian(type);

	switch (type)
	{
	case Anything::ANY_NULL:
		{
			return new NullAnythingI();
		}
	case Anything::ANY_BOOL:
		{
			return new BoolAnythingI(in, references);
		}
	case Anything::ANY_INT:
		{
			return new IntAnythingI(in, references);
		}
	case Anything::ANY_DOUBLE:
		{
			return new DoubleAnythingI(in, references);
		}
	case Anything::ANY_STRING:
		{
			return new StringAnythingI(in, references);
		}
	case Anything::ANY_UTF8STRING:
		{
			return new UTF8StringAnythingI(in, references);
		}
	case Anything::ANY_RAW:
		{
			return new RawAnythingI(in, references);
		}
	case Anything::ANY_STREAM:
		{
			return new StreamAnythingI(in, references);
		}
	case Anything::ANY_LIST:
		{
			return new ListAnythingI(in, references);
		}
	case Anything::ANY_MAP:
		{
			return new MapAnythingI(in, references);
		}
	case Anything::ANY_REFERENCE:
		{
			try
			{
				int tellg;
				in->readComplete((char*)&tellg, 0, sizeof(int));
				tellg = bigendian(tellg);
				AbstractAnythingI::validate(tellg);
				return Anything((AnythingI*) references->get(tellg));
			}
			catch (const Exception& e)
			{
				throw IOException(e->getMessage());
			}
			break;
		}
	case Anything::ANY_DUPLICATE:
		{
			int duplicates;
			in->read((char*)&duplicates, 0, sizeof(int));
			duplicates = bigendian(duplicates);
			AbstractAnythingI::validate(duplicates);	
			references->put(-1, 0); // just a hint for ListAnything
			return new IntAnythingI(duplicates);
		}
	default:
		{
			break;
		}
	}

	throw IOException(WITHDETAILS(L"InputStream does not contain an Anything."));
}

//********************************************************************

Anything::operator bool() const 
{
	return 0 != get() ? get()->operator bool() : false;
}

//********************************************************************

Anything::operator int() const 
{
	return 0 != get() ? get()->operator int() : 0;
}

//********************************************************************

Anything::operator double() const 
{
	return 0 != get() ? get()->operator double() : 0.;
}

//********************************************************************

Anything::operator String() const 
{
	return 0 != get() ? get()->operator String() : String();
}

//********************************************************************

Anything::operator UTF8String() const 
{
	return 0 != get() ? get()->operator UTF8String() : UTF8String();
}

//********************************************************************
//********************************************************************

std::ostream& operator<<(std::ostream& os, const Anything& anything)
{
	return os << anything->operator UTF8String();
}

//********************************************************************

std::wostream& operator<<(std::wostream& os, const Anything& anything)
{
	return os << anything->operator String(); 
}

//********************************************************************
//********************************************************************

bool operator==(const Anything& a, const Anything& b)
{
	return a->equals(b);
}

//********************************************************************

bool operator<(const Anything& a, const Anything& b)
{
	if (a->type() == b->type())
	{
		switch (a->type())
		{
		case Anything::ANY_BOOL:
			{
				return a->operator bool() != b->operator bool();
			}
		case Anything::ANY_INT:
			{
				return a->operator int() < b->operator int();
			}
		case Anything::ANY_DOUBLE:
			{
				return a->operator double() < b->operator double();
			}
		case Anything::ANY_STRING:
			{
				return a->operator String() < b->operator String();
			}
		case Anything::ANY_UTF8STRING:
			{
				return a->operator UTF8String() < b->operator UTF8String();
			}
		}
	}
	return false;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
