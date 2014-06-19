// Reference.h

//********************************************************************

#ifndef jlj_core_memorymanagement_Reference_h
#define jlj_core_memorymanagement_Reference_h

//********************************************************************

#include <jlj/core/platform/config.h>

//********************************************************************

#include <cstdlib>
#include <new>
#include <typeinfo.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(memorymanagement)

//********************************************************************

class ExportedByJLJcore ReferencedObjectI;

//********************************************************************

class ExportedByJLJcore GenericReference
{
public:
	typedef void (*DeleteFunction)(ReferencedObjectI* pointee);	

	typedef void (*ExceptionCallback)(const char*);

protected:
	~GenericReference();

	GenericReference(ReferencedObjectI* pointee, DeleteFunction);

	GenericReference(const GenericReference&);

	GenericReference& operator=(const GenericReference&);

protected:
	void swap(GenericReference&);

	void increment_strong_references() const;

	void decrement_strong_references() const;

	ReferencedObjectI* get() const;

	bool less(ReferencedObjectI*) const;

public:
	static ReferencedObjectI* get(const GenericReference& rhs) { return rhs.get(); }

	bool operator!() const;

	static ExceptionCallback exceptioncallback;

private:
	void* operator new(size_t);

private:
	class Body;
	Body* body;

};

//********************************************************************

template<typename pointee_type>
class /* ExportedByJLJcore */ TypedReference 
	: public GenericReference
{
public:	
	TypedReference(pointee_type* pointee) 
		: GenericReference(pointee, &TypedReference::doDelete)
	{}

	TypedReference(const TypedReference<pointee_type>& rhs)
		: GenericReference(rhs)
	{}

	TypedReference& operator=(const TypedReference<pointee_type>& rhs)
	{
		GenericReference::operator=(rhs);
		return *this;
	}

	void swap(TypedReference& with) 
	{
		GenericReference::swap(with);
	}

	template<typename rhs_pointee_type>
	bool operator==(const TypedReference<rhs_pointee_type>& rhs) const
	{
		if (get() == get(rhs))
		{
			return true;
		}
		else
		{
			return dynamic_cast<pointee_type*>(get())->equals(dynamic_cast<rhs_pointee_type*>(get(rhs)));
		}
	}

	template<typename rhs_pointee_type>
	bool operator!=(const TypedReference<rhs_pointee_type>& rhs) const
	{
		if (get() == get(rhs))
		{
			return false;
		}
		else
		{
			return !dynamic_cast<pointee_type*>(get())->equals(dynamic_cast<rhs_pointee_type*>(get(rhs)));
		}
	}

	template<typename rhs_pointee_type>
	bool operator<(const TypedReference<rhs_pointee_type>& rhs) const
	{
		return less(get(rhs));
	}

#ifdef WIN32
public:
#else
protected:
#endif
	void increment_strong_references() const
	{
		GenericReference::increment_strong_references();
	}

private:
	static void doDelete(ReferencedObjectI* pointee) 
	{
		delete dynamic_cast<pointee_type*>(pointee);
	}

};

//********************************************************************

template<typename pointee_type>
class /* ExportedByJLJcore */ StrongReference 
	: public TypedReference<pointee_type>
{
public:		
	typedef pointee_type pointeetype;

	typedef TypedReference<pointee_type> base_type;

	~StrongReference()
	{
		this->decrement_strong_references();
	}

	StrongReference() 
		: base_type(static_cast<pointee_type*>(0)) 
	{
		this->increment_strong_references();
	}
	StrongReference(pointee_type* p) 
		: base_type(p) 
	{
		this->increment_strong_references();
	}

	StrongReference(const pointee_type* p) 
		: base_type(const_cast<pointee_type*>(p)) 
	{
		this->increment_strong_references();
	}

	StrongReference(const StrongReference& rhs)
		: base_type(rhs)
	{
		this->increment_strong_references();
	}

	StrongReference(const TypedReference<pointee_type>& rhs)
		: base_type(rhs)
	{
		this->increment_strong_references();
	}

	pointee_type* get() const 
	{
		return dynamic_cast<pointee_type*>(base_type::get());
	}

	pointee_type* operator->() const
	{
		ReferencedObjectI* body = base_type::get();
		if (0 == body) 
		{
			GenericReference::exceptioncallback(typeid(pointee_type).name());
			return 0;
		}
		else
		{
			return dynamic_cast<pointee_type*>(body);
		}
	}

	pointee_type& operator*() const
	{
		ReferencedObjectI* body = base_type::get();
		if (0 == body) 
		{
			GenericReference::exceptioncallback(typeid(pointee_type).name());
			return *((pointee_type*) 0);
		}
		else
		{
			return *dynamic_cast<pointee_type*>(body);
		}
	}

	StrongReference& operator=(const base_type& rhs)
	{
		if (this != &rhs)
		{
			rhs.increment_strong_references();
			this->decrement_strong_references();
			base_type::operator=(rhs);
		}
		return *this;
	}

	StrongReference& operator=(const StrongReference& rhs)
	{
		if (this != &rhs)
		{
			rhs.increment_strong_references();
			this->decrement_strong_references();
			base_type::operator=(rhs);
		}
		return *this;
	}

	StrongReference& operator=(pointee_type* p)
	{
		reset(p);
		return *this;
	}

	void reset(pointee_type* p)
	{
		if (get() != p)
		{
			StrongReference tmp(p);
			base_type::swap(tmp);
		}
	}

	void swap(base_type& with) 
	{
		base_type::swap(with);
	}

};

//********************************************************************

template<typename pointee_type>
class /* ExportedByJLJcore */ WeakReference 
	: public TypedReference<pointee_type>
{
public:		
	typedef TypedReference<pointee_type> base_type;

	~WeakReference() 
	{}

	WeakReference() 
		: base_type(static_cast<pointee_type*>(0)) 
	{}

	WeakReference(const TypedReference<pointee_type>& rhs)
		: base_type(rhs)
	{}

	pointee_type* get() const 
	{
		return dynamic_cast<pointee_type*>(base_type::get());
	}

	pointee_type* operator->() const
	{
		ReferencedObjectI* body = base_type::get();
		if (0 == body) 
		{
			GenericReference::exceptioncallback(typeid(pointee_type).name());
			return 0;
		}
		else
		{
			return dynamic_cast<pointee_type*>(body);
		}
	}

	pointee_type& operator*() const
	{
		ReferencedObjectI* body = base_type::get();
		if (0 == body) 
		{
			GenericReference::exceptioncallback(typeid(pointee_type).name());
			return *((pointee_type*) 0);
		}
		else
		{
			return *dynamic_cast<pointee_type*>(body);
		}
	}


	WeakReference& operator=(const TypedReference<pointee_type>& rhs)
	{
		base_type::operator=(rhs);
		return *this;
	}

	void reset(pointee_type* p)
	{
		base_type tmp(p);
		base_type::swap(tmp);
	}

	void swap(base_type& with) 
	{
		base_type::swap(with);
	}

};

//********************************************************************
//********************************************************************

#define StrongReferenceCommonDerivateImplementation(classname) \
	typedef classname##I pointee_type; \
	\
	typedef StrongReference< pointee_type > base_type; \
	\
	~##classname() \
{} \
	\
	classname()  \
	: base_type(static_cast<pointee_type*>(0))  \
{} \
	\
	classname(pointee_type* p)  \
	: base_type(p)  \
{} \
	\
	classname(const pointee_type* p)  \
	: base_type(p)  \
{} \
	\
	classname(const classname& rhs) \
	: base_type(rhs) \
{} \
	\
	template< typename U > \
	classname(const StrongReference<U>& rhs) \
	: base_type(rhs.get()) \
{} \
	\
	template< typename U > \
	classname(const WeakReference<U>& rhs) \
	: base_type(rhs.get()) \
{} \
	\
	classname(const TypedReference<pointee_type>& rhs) \
	: base_type(rhs) \
{} \
	\
	template< typename U > \
	classname& operator=(const StrongReference<U>& rhs) \
{ \
	pointee_type* base = rhs.get();	\
	base_type::operator=(base); \
	return *this; \
} \
	\
	template< typename U > \
	classname& operator=(const WeakReference<U>& rhs) \
{ \
	pointee_type* base = rhs.get();	\
	base_type::operator=(base); \
	return *this; \
} \
	\
	classname& operator=(const TypedReference<pointee_type>& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(const base_type& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(const classname& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(pointee_type* p) \
{ \
	base_type::operator=(p); \
	return *this; \
} \
	\
	classname& operator=(const pointee_type* p) \
{ \
	base_type::operator=(p); \
	return *this; \
} \

//********************************************************************

#define StrongReferenceTemplateDerivateImplementation(classname, type) \
	typedef classname##I<type> pointee_type; \
	\
	typedef StrongReference< pointee_type > base_type; \
	\
	~##classname() \
{} \
	\
	classname()  \
	: base_type(static_cast<pointee_type*>(0))  \
{} \
	\
	classname(pointee_type* p)  \
	: base_type(p)  \
{} \
	\
	classname(const pointee_type* p)  \
	: base_type(p)  \
{} \
	\
	classname(const classname& rhs) \
	: base_type(rhs) \
{} \
	\
	template< typename U > \
	classname(const StrongReference<U>& rhs) \
	: base_type(rhs.get()) \
{} \
	\
	template< typename U > \
	classname(const WeakReference<U>& rhs) \
	: base_type(rhs.get()) \
{} \
	\
	classname(const TypedReference<pointee_type>& rhs) \
	: base_type(rhs) \
{} \
	\
	template< typename U > \
	classname& operator=(const StrongReference<U>& rhs) \
{ \
	pointee_type* base = rhs.get(); \
	base_type::operator=(base); \
	return *this; \
} \
	\
	template< typename U > \
	classname& operator=(const WeakReference<U>& rhs) \
{ \
	pointee_type* base = rhs.get(); \
	base_type::operator=(base); \
	return *this; \
} \
	\
	classname& operator=(const TypedReference<pointee_type>& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(const base_type& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(const classname& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(pointee_type* p) \
{ \
	base_type::operator=(p); \
	return *this; \
} \
	\
	classname& operator=(const pointee_type* p) \
{ \
	base_type::operator=(p); \
	return *this; \
} \

//********************************************************************

#define StrongReferenceTemplate2DerivateImplementation(classname, typeK, typeV) \
	typedef classname##I<typeK, typeV> pointee_type; \
	\
	typedef StrongReference< pointee_type > base_type; \
	\
	~##classname() \
{} \
	\
	classname()  \
	: base_type(static_cast<pointee_type*>(0))  \
{} \
	\
	classname(pointee_type* p)  \
	: base_type(p)  \
{} \
	\
	classname(const pointee_type* p)  \
	: base_type(p)  \
{} \
	\
	classname(const classname& rhs) \
	: base_type(rhs) \
{} \
	\
	template< typename T > \
	classname(const StrongReference<T>& rhs) \
	: base_type(rhs.get()) \
{} \
	\
	template< typename T > \
	classname(const WeakReference<T>& rhs) \
	: base_type(rhs.get()) \
{} \
	\
	classname(const TypedReference<pointee_type>& rhs) \
	: base_type(rhs) \
{} \
	\
	template< typename T > \
	classname& operator=(const StrongReference<T>& rhs) \
{ \
	pointee_type* base = rhs.get(); \
	base_type::operator=(base); \
	return *this; \
} \
	\
	template< typename T > \
	classname& operator=(const WeakReference<T>& rhs) \
{ \
	pointee_type* base = rhs.get(); \
	base_type::operator=(base); \
	return *this; \
} \
	\
	classname& operator=(const TypedReference<pointee_type>& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(const base_type& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(const classname& rhs) \
{ \
	base_type::operator=(rhs); \
	return *this; \
} \
	\
	classname& operator=(pointee_type* p) \
{ \
	base_type::operator=(p); \
	return *this; \
} \
	\
	classname& operator=(const pointee_type* p) \
{ \
	base_type::operator=(p); \
	return *this; \
} \

//********************************************************************
//********************************************************************

#define StrongReferenceCommonDeclaration(classname, EXPORTDECLARATION) \
struct EXPORTDECLARATION classname : StrongReference<classname##I> \
{ \
	StrongReferenceCommonDerivateImplementation(classname); \
	\
}

//********************************************************************
//********************************************************************

// jlj/lang/ClassLoader.h for details

#define ClassLoaderInformationDeclaration(fullclassname, md5fromclassname, EXPORTDECLARATION) \
	extern "C" { \
	EXPORTDECLARATION jlj::lang::ObjectI* instantiate_##md5fromclassname(); \
}

//********************************************************************

#define ClassLoaderInformationDefinition(fullclassname, md5fromclassname) \
	extern "C" { \
	jlj::lang::ObjectI* instantiate_##md5fromclassname() \
{ \
	try \
{ \
	return new fullclassname##I(); \
} \
	catch (...) \
{ \
	return 0; \
} \
} \
}

//********************************************************************

NAMESPACE_END(memorymanagement)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
