// UnsynchronizedReference.h

//********************************************************************

#ifndef jlj_core_memorymanagement_UnsynchronizedReference
#define jlj_core_memorymanagement_UnsynchronizedReference

//********************************************************************

/**
* @reference http://www.octopull.demon.co.uk/arglib/
*/

//********************************************************************

#include <jlj/core/platform/config.h>

//********************************************************************

#include <stdlib.h>

//********************************************************************

/**
* Enable this define to print the typename when deleting an unsynchronized ReferenceObject.
* This might fail because it is accessed from more than one thread.
* In this case the UnsynchronizedReference<this type> should be made a Reference which is synchronized.
*/

#undef DEBUG_DELETE_PROBLEM
// #define DEBUG_DELETE_PROBLEM

#ifdef DEBUG_DELETE_PROBLEM
#include <stdio.h>
#include <typeinfo.h>
#endif

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(memorymanagement)

//********************************************************************

class ExportedByJLJcore ReferencedObjectI;

//********************************************************************

class ExportedByJLJcore GenericUnsynchronizedReference
{
public:
	typedef void (*DeleteFunction)(ReferencedObjectI* pointee);

	typedef void (*ExceptionCallback)(const char*);

protected:
	~GenericUnsynchronizedReference();

	GenericUnsynchronizedReference(ReferencedObjectI* pointee, DeleteFunction);

	GenericUnsynchronizedReference(const GenericUnsynchronizedReference&);

	GenericUnsynchronizedReference& operator=(const GenericUnsynchronizedReference&);

protected:
	void swap(GenericUnsynchronizedReference&);

	void increment_strong_references() const;

	void decrement_strong_references() const;

	ReferencedObjectI* get() const;

	bool less(ReferencedObjectI*) const;

public:
	static ReferencedObjectI* get(const GenericUnsynchronizedReference& rhs) { return rhs.get(); }

	bool operator!() const;

	static ExceptionCallback exceptioncallback;

private:
	class Body;

	Body* body;

};

//********************************************************************

template<typename pointee_type>
class /* ExportedByJLJcore */ TypedUnsynchronizedReference 
	: public GenericUnsynchronizedReference
{
public:	
	TypedUnsynchronizedReference(pointee_type* pointee) 
		: GenericUnsynchronizedReference(pointee, &TypedUnsynchronizedReference::doDelete)
	{}

	TypedUnsynchronizedReference(const TypedUnsynchronizedReference<pointee_type>& rhs)
		: GenericUnsynchronizedReference(rhs)
	{}

	TypedUnsynchronizedReference& operator=(const TypedUnsynchronizedReference<pointee_type>& rhs)
	{
		GenericUnsynchronizedReference::operator=(rhs);
		return *this;
	}

	void swap(TypedUnsynchronizedReference& with) 
	{
		GenericUnsynchronizedReference::swap(with);
	}

	template<typename rhs_pointee_type>
	bool operator==(const TypedUnsynchronizedReference<rhs_pointee_type>& rhs) const
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
	bool operator!=(const TypedUnsynchronizedReference<rhs_pointee_type>& rhs) const
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
	bool operator<(const TypedUnsynchronizedReference<rhs_pointee_type>& rhs) const
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
		GenericUnsynchronizedReference::increment_strong_references();
	}

private:
	static void doDelete(ReferencedObjectI* pointee) 
	{
#ifdef DEBUG_DELETE_PROBLEM
		fprintf(stdout, "before : %s\n", typeid(pointee_type).name());
#endif
		delete dynamic_cast<pointee_type*>(pointee);
#ifdef DEBUG_DELETE_PROBLEM
		fprintf(stdout, "after  : %s\n", typeid(pointee_type).name());
#endif
	}

};

//********************************************************************

template<typename pointee_type>
class /* ExportedByJLJcore */ StrongUnsynchronizedReference 
	: public TypedUnsynchronizedReference<pointee_type>
{
public:		
	typedef TypedUnsynchronizedReference<pointee_type> base_type;

	~StrongUnsynchronizedReference()
	{
		this->decrement_strong_references();
	}

	StrongUnsynchronizedReference() 
		: base_type(static_cast<pointee_type*>(0)) 
	{
		this->increment_strong_references();
	}

	StrongUnsynchronizedReference(pointee_type* p) 
		: base_type(p) 
	{
		this->increment_strong_references();
	}

	StrongUnsynchronizedReference(const pointee_type* p) 
		: base_type(const_cast<pointee_type*>(p)) 
	{
		this->increment_strong_references();
	}

	StrongUnsynchronizedReference(const StrongUnsynchronizedReference& rhs)
		: base_type(rhs)
	{
		this->increment_strong_references();
	}

	StrongUnsynchronizedReference(const TypedUnsynchronizedReference<pointee_type>& rhs)
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
			GenericUnsynchronizedReference::exceptioncallback(typeid(pointee_type).name());
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
			GenericUnsynchronizedReference::exceptioncallback(typeid(pointee_type).name());
			return *((pointee_type*) 0);
		}
		else
		{
			return *dynamic_cast<pointee_type*>(body);
		}
	}

	StrongUnsynchronizedReference& operator=(const base_type& rhs)
	{
		if (this != &rhs)
		{
			rhs.increment_strong_references();
			this->decrement_strong_references();
			base_type::operator=(rhs);
		}
		return *this;
	}

	StrongUnsynchronizedReference& operator=(const StrongUnsynchronizedReference& rhs)
	{
		if (this != &rhs)
		{
			rhs.increment_strong_references();
			this->decrement_strong_references();
			base_type::operator=(rhs);
		}
		return *this;
	}

	StrongUnsynchronizedReference& operator=(pointee_type* p)
	{
		reset(p);
		return *this;
	}

	void reset(pointee_type* p)
	{
		if (get() != p)
		{
			StrongUnsynchronizedReference tmp(p);
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
class /* ExportedByJLJcore */ WeakUnsynchronizedReference 
	: public TypedUnsynchronizedReference<pointee_type>
{
public:		
	typedef TypedUnsynchronizedReference<pointee_type> base_type;

	~WeakUnsynchronizedReference() 
	{}

	WeakUnsynchronizedReference() 
		: base_type(static_cast<pointee_type*>(0)) 
	{}

	WeakUnsynchronizedReference(const TypedUnsynchronizedReference<pointee_type>& rhs)
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
			GenericUnsynchronizedReference::exceptioncallback(typeid(pointee_type).name());
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
			GenericUnsynchronizedReference::exceptioncallback(typeid(pointee_type).name());
			return *((pointee_type*) 0);
		}
		else
		{
			return *dynamic_cast<pointee_type*>(body);
		}
	}

	WeakUnsynchronizedReference& operator=(const TypedUnsynchronizedReference<pointee_type>& rhs)
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

#define StrongUnsynchronizedReferenceCommonDerivateImplementation(classname) \
	typedef classname##I pointee_type; \
	\
	typedef StrongUnsynchronizedReference< pointee_type > base_type; \
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
	classname(const TypedUnsynchronizedReference<pointee_type>& rhs) \
	: base_type(rhs) \
{} \
	\
	classname& operator=(const TypedUnsynchronizedReference<pointee_type>& rhs) \
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
	operator Object() \
{ \
	return get(); \
} \
	operator Object() const \
{ \
	return get(); \
} \

//********************************************************************
//********************************************************************

#define StrongUnsynchronizedReferenceCommonDerivedImplementation(classname, base_type) \
	typedef classname##I pointee_type; \
	\
	explicit classname(pointee_type* p)  \
	: base_type(p)  \
{} \

//********************************************************************

NAMESPACE_END(memorymanagement)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
