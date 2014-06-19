// StreamAnything.h

//********************************************************************

#ifndef jlj_util_StreamAnything
#define jlj_util_StreamAnything

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil StreamAnythingI 
	: public AbstractAnythingI
{
protected:
	~StreamAnythingI() throw (IOException);
	friend class TypedReference<StreamAnythingI>;

public:
	StreamAnythingI(InputStream) throw (IOException);
	StreamAnythingI(InputStream&, Map<int, void*>&) throw (IOException);

public:
	int type() const;

	InputStream getInputStream() const throw (IOException);

	/**
	* If this StreamAnythingI was deserialized from an InputStream it holds the internal
	* InputStream in a temporay file. In this case "saveInputStreamToFileOutputStream"
	* will just move/rename the tempfile to the destination filepath. If the move fails
	* because the OS doesn't support the specific command (see IOException) then use 
	* "getInputStream" to read it into a new OutputStream.
	* In any other case the InputStream is copied into a new FileOutputStream.
	*/
	void saveInputStreamToFileOutputStream(const String& filepath) throw (IOException);

	/**
	* clone() returns a shallow copy, 
	* i.e. modifications on the internal streams will affect the original and the clone
	*/
	Object clone() const;

	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

	/**
	* returns tell() of the underlying InputStream
	*/
	int size() const;

private:
	void pipe(InputStream, OutputStream) const throw (IOException);

	mutable InputStream inputstream;
	mutable int streamedbytes;

	String temporaryfilepath;

	static const int DEFAULTBUFFERSIZE;

};

//********************************************************************

StrongReferenceCommonDeclaration(StreamAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
