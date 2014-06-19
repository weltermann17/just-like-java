// Hash.h

//********************************************************************

#ifndef Hash_h
#define Hash_h

//********************************************************************

#include <jlj/core/hashing/hashfunc.h> 
#include <jlj/core/ptypes/pport.h> 
#include <sstream>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(core)
NAMESPACE_BEGIN(hashing)

//********************************************************************

/**
* use the template below to use a type as class ExportedByJLJcore K(ey) in a map,
* a typical example are the built-in types int, double etc.
* for these a version of hashFunction is provided
*/

//********************************************************************

#define _hash(h) h += ~(h << 9); h ^= (h >> 14); h += (h << 4); h ^= (h >> 10)

//********************************************************************

inline pt::large unsignedRightShift(pt::large value, int steps)
{
	return (0 <= value) ? (value >> steps) : ((value >> steps) + (2 << ~steps)); 
}

//********************************************************************

inline int hashFunction(char t) { int tt = t; return _hash(tt); }
inline int hashFunction(unsigned char t) { int tt = t; return _hash(tt); }
inline int hashFunction(short t) { int tt = t; return _hash(tt); }
inline int hashFunction(unsigned short t) { int tt = t; return _hash(tt); }
inline int hashFunction(int t) { return _hash(t); }
inline int hashFunction(unsigned int t) { return _hash(t); }
inline int hashFunction(float t) { return _hash(*((int*)&t)); }
inline int hashFunction(double t) { pt::large bits = *((pt::large*)&t); bits ^= unsignedRightShift(bits, 32); int h = (int)bits; return _hash(h); }
#if defined(PLATEFORME_DS64)
inline int hashFunction(void* t) { return hash1((unsigned char*) t, sizeof(void*), 1, 0); }
inline int hashFunction(const void* t) { return hash1((unsigned char*) t, sizeof(const void*), 1, 0); }
#else
inline int hashFunction(void* t) { int h = (int) t; return _hash(h); }
inline int hashFunction(const void* t) { int h = (int) t; return _hash(h); }
#endif

#undef _hash

//********************************************************************

/**
* example: 
*
* typedef HashAdaptor<int> hashint;
* Map<hashint, String> map = new HashMap<hashint, String>();
*
* implement your own adaptor in order to override hashCode with something
* more appropriate if necessary
*/

template< typename T >
class /* ExportedByJLJcore */ HashAdaptor
{
public:
	inline HashAdaptor() {}
	inline HashAdaptor(T t) : t(t) {}
	inline int hashCode() const { return hashFunction(t); }
	inline HashAdaptor<T>* operator->() { return this; } 
	inline HashAdaptor<T>* operator->() const { return (HashAdaptor<T>*)this; } 
	inline operator T() { return t; }
	inline operator T() const { return t; }	
	std::wstring toString() const 
	{ 
		std::wostringstream s;
		s << hashCode();
		return s.str();
	}

public:
	T t;

};

//********************************************************************

template< typename T >
bool operator==(const HashAdaptor<T>& a, const HashAdaptor<T>& b)
{
	return a.t == b.t;
}
				
//********************************************************************

NAMESPACE_END(hashing)
NAMESPACE_END(core)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
