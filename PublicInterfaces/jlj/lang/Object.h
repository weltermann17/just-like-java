// ObjectI.h

//********************************************************************

#ifndef jlj_lang_Object_h
#define jlj_lang_Object_h

//********************************************************************

#include <JLJlang.h>

//********************************************************************

#include <jlj/core/memorymanagement/ReferencedObject.h>
using namespace jlj::core::memorymanagement;

//********************************************************************

#include <jlj/core/language/Final.h>

//********************************************************************

#include <iostream>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang String;
struct ExportedByJLJlang Class;
class ExportedByJLJlang ClassCastException;

//********************************************************************

class ExportedByJLJlang ObjectI;
struct ExportedByJLJlang Object;

//********************************************************************

class ExportedByJLJlang ObjectI
	: public virtual ReferencedObjectI
{
protected:
	~ObjectI();
	friend class TypedReference<ObjectI>;

public:
	ObjectI();

public:
	virtual bool equals(const Object&) const;
	virtual int hashCode() const;
	virtual String toString() const;
	virtual Class getClass() const;
	/**
	* if not otherwise stated clone() always returns a shallow copy
	*/
	virtual Object clone() const;


	template< typename T > bool instanceOf(StrongReference<T>& ref) const
	{
		T* derived = const_cast<T*>(dynamic_cast< const T*>(this)); 

		if (0 != derived) 
		{
			ref = StrongReference<T>(derived);
			return true;
		}
		else
		{
			ref.reset(0);
			return false;
		}
	}

	template< typename T > bool instanceOf(StrongUnsynchronizedReference<T>& ref) const
	{
		T* derived = const_cast<T*>(dynamic_cast< const T*>(this)); 

		if (0 != derived) 
		{
			ref = StrongUnsynchronizedReference<T>(derived);
			return true;
		}
		else
		{
			ref.reset(0);
			return false;
		}
	}

	/**
	* make sure to #include <jlj/lang/Throwable> before using downcast()
	*/
	template< typename T > void downcast(StrongReference<T>& ref) const throw (ClassCastException)
	{
		T* derived = const_cast<T*>(dynamic_cast< const T*>(this)); 

		if (0 != derived) 
		{
			ref = StrongReference<T>(derived);
		}
		else
		{
			ref.reset(0);
			throw ClassCastException(
				computeClassCastMessage(
				typeid(*this).name(), 
				typeid(T).name()));
		}
	}

	template< typename T > void downcast(StrongUnsynchronizedReference<T>& ref) const throw (ClassCastException)
	{
		T* derived = const_cast<T*>(dynamic_cast< const T*>(this)); 

		if (0 != derived) 
		{
			ref = StrongUnsynchronizedReference<T>(derived);
		}
		else
		{
			ref.reset(0);
			throw ClassCastException(
				computeClassCastMessage(
				typeid(*this).name(), 
				typeid(T).name()));
		}
	}

protected:
	static Class makeClass(const char*);

	template< typename T > Class getClass(const T& derived) const
#ifdef WIN32
		;
#else
	{
		return makeClass(typeid(derived).name());
	}	
#endif

private:
	static String computeClassCastMessage(const char* from, const char* to);

};

//********************************************************************
//********************************************************************

StrongReferenceCommonDeclaration(Object, ExportedByJLJlang);

//********************************************************************
//********************************************************************

template< typename T, typename S > 
bool operator>(const T& a, const S& b)
{
	return !(a <= b);
}

//********************************************************************

template< typename T, typename S > 
bool operator<=(const T& a, const S& b)
{
	return a == b || a < b;
}

//********************************************************************

template< typename T, typename S > 
bool operator>=(const T& a, const S& b)
{
	return !(a < b);
}

//********************************************************************
//********************************************************************

template< typename T > 
/* ExportedByJLJlang */ std::ostream& operator<<(std::ostream& os, const StrongReference<T>& o)
{
	return os << o->toString()->toUTF8String();
}

//********************************************************************

template< typename T > 
/* ExportedByJLJlang */ std::wostream& operator<<(std::wostream& os, const Object& o)
{
	return os << o->toString();
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

//********************************************************************

// eof
