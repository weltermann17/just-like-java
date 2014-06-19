// Anything.h

//********************************************************************

#ifndef jlj_util_Anything_h
#define jlj_util_Anything_h

//********************************************************************

#include <jlj/lang/Class.h>
#include <jlj/lang/UTF8String.h>
#include <jlj/lang/UTF8StringBuffer.h>
using namespace jlj::lang;

#include <jlj/io/Reader.h>
#include <jlj/io/Writer.h>
#include <jlj/io/OutputStream.h>
#include <jlj/io/InputStream.h>
using namespace jlj::io;

#include <jlj/util/List.h>
#include <jlj/util/Map.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

struct ExportedByJLJutil Anything;

//********************************************************************

class ExportedByJLJutil AnythingI
	: public virtual ListI<Anything>
	, public virtual MapI<String, Anything>
{
public:
	~AnythingI() = 0;
	AnythingI();

public:
	virtual bool exists() const = 0;
	virtual bool isNull() const = 0;
	virtual int type() const = 0;

	virtual Object clone() const = 0;  // deep copy for all subtypes of Anything

public:
	virtual	operator bool() const = 0;
	virtual	operator int() const = 0;
	virtual	operator double() const = 0;
	virtual	operator String() const = 0;
	virtual	operator UTF8String() const = 0;

	virtual InputStream getInputStream() const throw (IOException) = 0;

public:
	virtual void serialize(OutputStream) const = 0;
	virtual void write(Writer) const = 0;
	virtual bool writeXml(Writer) const = 0;

public:
	virtual bool add(const Anything&) = 0;
	virtual void add(int index, const Anything&) throw (IndexOutOfBoundsException) = 0;	 
	virtual bool addAll(const Collection<Anything>&) = 0;
	virtual Anything get(int index) const throw (IndexOutOfBoundsException) = 0;
	virtual Anything get(const String& key) const throw (IndexOutOfBoundsException) = 0;

	virtual Anything remove(const String& key) throw (IndexOutOfBoundsException) = 0;
	virtual bool remove(const Anything&) = 0;

	virtual int size() const = 0;

	virtual UTF8String toUTF8String() const = 0;
	virtual UTF8String toBase64(bool zip) const throw (IOException) = 0;
	virtual UTF8String toJSON(int sizehint = 0) const throw (IOException) = 0;

public:
	virtual void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException) = 0;
	virtual void write__(Writer&, int level, Map<void*, int>&) const throw (IOException) = 0;
	virtual void writeJSON__(UTF8StringBuffer&) const throw (IOException) = 0;

};

//********************************************************************

struct ExportedByJLJutil Anything : StrongReference<AnythingI>
{    
	StrongReferenceCommonDerivateImplementation(Anything);

	Anything(bool);
	Anything(int);
	Anything(double);
	Anything(const UTF8String&);
	Anything(const String&);
	Anything(char*, int length, bool ownership, bool copy); // RawAnything
	Anything(InputStream); // StreamAnything
	Anything(List<Anything>);
	Anything(Map<String, Anything>);

	static Anything decodeInputStream(InputStream) throw (IOException);
	static Anything decodeBase64(const UTF8String& base64formattedutf8string) throw (IOException);
	static Anything decodeJSON(const UTF8String& jsonstring, bool errorhandling = false) throw (IOException);

	enum 
	{ 
		ANY_UNKNOWN = 0xFF,

		ANY_NULL = 0x00,
		ANY_BOOL,
		ANY_INT,
		ANY_DOUBLE,
		ANY_STRING,
		ANY_UTF8STRING,
		ANY_RAW,
		ANY_STREAM,

		ANY_LIST = 0x40,
		ANY_MAP,

		ANY_DUPLICATE = 0x7E,
		ANY_REFERENCE = 0x7F,

		MAGICCOOKIE_LO = 0x414E5954,	// ANYT
		MAGICCOOKIE_HI = 0x48494E47		// HING
	};

	operator bool() const;
	operator int() const;
	operator double() const;
	operator String() const;
	operator UTF8String() const;

	static Anything deserialize__(InputStream&, Map<int, void*>&) throw (IOException);

};

//********************************************************************
//********************************************************************

ExportedByJLJutil bool operator==(const Anything&, const Anything&);
ExportedByJLJutil bool operator<(const Anything&, const Anything&);

//********************************************************************

ExportedByJLJutil std::ostream& operator<<(std::ostream&, const Anything&);
ExportedByJLJutil std::wostream& operator<<(std::wostream&, const Anything&);

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
