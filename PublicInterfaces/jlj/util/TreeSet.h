// TreeSet.h

//********************************************************************

#ifndef TreeSet_h
#define TreeSet_h

//********************************************************************

#include <jlj/util/TreeSetBase.h>
#include <jlj/util/TreeSetIterator.h>
#include <jlj/util/TreeSetFromTo.h>
#include <jlj/util/Comparator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class TreeSetI 
	: public TreeSetBaseI<T>
{
protected:
	~TreeSetI();
	friend class TypedReference<TreeSetI<T> >;

public:
	TreeSetI();
	TreeSetI(const Collection<T>&);	
	TreeSetI(const SortedSet<T>&);	
	TreeSetI(const Comparator<T>&);	

public:
	bool add(const T&);
	void clear();
	bool contains(const T&) const;
	Iterator<T> iterator();
	Iterator<T> iterator() const;
	bool remove(const T&);
	int size() const;

	const T& first() const;
	SortedSet<T> headSet(const T& to) throw (IndexOutOfBoundsException);
	const T& last() const;
	SortedSet<T> subSet(const T& from, const T& to) throw (IndexOutOfBoundsException);
	SortedSet<T> tailSet(const T& from) throw (IndexOutOfBoundsException);
	Comparator<T> comparator() const;

public:
	TreeNode<T>* getRoot() const;
	TreeNode<T>* lookupNode(const T&) const;
	T& lookup(const T&) const;

	struct lessthancomparator : public ComparatorI<T>
	{
		inline int compare(const T& a, const T& b) const { return a < b ? -1 : a == b ? 0 : 1; }
	};

private:
	int replace(const T&);

	void add__(const T&, TreeNode<T>*&, TreeNode<T>*, int&);
	void remove__(const T&, TreeNode<T>*&, int&, int&);
	void balanceleft__(TreeNode<T>*&, int&);
	void balanceright__(TreeNode<T>*&, int&);
	void remove___(TreeNode<T>*&, TreeNode<T>*&, int&);
	TreeNode<T>* contains__(const T&, TreeNode<T>*) const;
	T& lookup__(const T&, TreeNode<T>*) const;
	void preorder__(TreeNode<T>*, TreeNode<T>*&);

	TreeNode<T>* root;
	int elements;
	const Comparator<T> comp;

	const T* sentinel;

};

//********************************************************************

template< typename T >
struct TreeSet : StrongReference<TreeSetI<T> > 
{
	StrongReferenceTemplateDerivateImplementation(TreeSet, T);
};

//********************************************************************
//********************************************************************

template< typename T >
TreeNode<T>::~TreeNode() 
{
	delete left; 
	delete right; 
}

//********************************************************************

template< typename T >
TreeNode<T>::TreeNode(const T& t, TreeNode<T>* up_) 
: left(0)
, right(0)
, up(up_)
, balance(0)
, data(t) 
{}

//********************************************************************

template< typename T >
TreeNode<T>::TreeNode(const T& t, TreeNode<T>* up_, int b_) 
: left(0)
, right(0)
, up(up_)
, balance(b_)
, data(t) 
{}

//********************************************************************

template< typename T >
TreeNode<T>*TreeNode<T>::next() 
{
	TreeNode<T>* p = this;
	if (p->right) 
	{
		p = p->right;
		while (p->left)
			p = p->left;
		return p;
	}
	else 
	{
		while ((p->up) && (p == p->up->right))
			p = p->up;
		return p->up;
	}
}

//********************************************************************

template< typename T >
TreeNode<T>* TreeNode<T>::prev() 
{
	TreeNode<T>* p = this;
	if (p->left) 
	{
		p = p->left;
		while (p->right)
			p = p->right;
		return p;
	}
	else 
	{
		while ((p->up) && (p == p->up->left))
			p = p->up;
		return p->up;
	}
}

//********************************************************************
//********************************************************************

template< typename T > 
TreeSetI<T>::~TreeSetI() 
{
	clear();
}

//********************************************************************

template< typename T > 
TreeSetI<T>::TreeSetI() 
: root(0)
, elements(0)
, sentinel(0)
{}

//********************************************************************

template< typename T > 
TreeSetI<T>::TreeSetI(const Collection<T>& c) 
: root(0)
, elements(0)
, sentinel(0)
{
	this->addAll(c);
}

//********************************************************************

template< typename T > 
TreeSetI<T>::TreeSetI(const SortedSet<T>& s) 
: root(0)
, elements(0)
, comp(s->comparator())
, sentinel(0)
{
	this->addAll(s);
}

//********************************************************************

template< typename T > 
TreeSetI<T>::TreeSetI(const Comparator<T>& comp) 
: root(0)
, elements(0)
, comp(comp)
, sentinel(0)
{}

//********************************************************************

template< typename T > 
bool TreeSetI<T>::add(const T& element) 
{
	int before = elements;
	int grown = 0;
	add__(element, root, 0, grown);
	return before < elements;
}

//********************************************************************

template< typename T > 
void TreeSetI<T>::clear()
{
	delete root; 
	root = 0;
	elements = 0;
}

//********************************************************************

template< typename T >
Comparator<T> TreeSetI<T>::comparator() const 
{
	return 0 == comp.get() ? new lessthancomparator() : comp;
}

//********************************************************************

template< typename T >
bool TreeSetI<T>::contains(const T& element) const 
{
	return 0 != contains__(element, root);
}

//********************************************************************

template< typename T > 
TreeNode<T>* TreeSetI<T>::getRoot() const
{
	return root;
}

//********************************************************************

template< typename T > 
Iterator<T> TreeSetI<T>::iterator()  
{
	return new TreeSetIteratorI<T>(this);
}

//********************************************************************

template< typename T > 
Iterator<T> TreeSetI<T>::iterator() const
{
	return new TreeSetIteratorI<T>((TreeSetBaseI<T>*) this);
}

//********************************************************************

template< typename T > 
bool TreeSetI<T>::remove(const T& element)
{
	int before = elements;
	int shrunk = 0;
	int removed = 0;
	remove__(element, root, shrunk, removed);
	return before > elements;
}

//********************************************************************

template< typename T > 
int TreeSetI<T>::size() const
{
	return elements;
}

//********************************************************************

template< typename T > 
const T& TreeSetI<T>::first() const
{
	TreeNode<T>* p = root;
	while (p->left)
		p = p->left;
	if (p)
		return p->data;
	else
		return *(T*)0;
}

//********************************************************************

template< typename T > 
SortedSet<T> TreeSetI<T>::headSet(const T& to) throw (IndexOutOfBoundsException)
{
	return subSet(first(), to);
}

//********************************************************************

template< typename T > 
const T& TreeSetI<T>::last() const
{
	TreeNode<T>* p = root;
	while (p->right)
		p = p->right;
	if (p)
		return p->data;
	else
		return *(T*)0;
}

//********************************************************************

template< typename T > 
SortedSet<T> TreeSetI<T>::subSet(const T& from, const T& to) throw (IndexOutOfBoundsException)
{	
	T f;
	T t;

	bool hasfrom = false;
	bool hasto = false;
	bool includeto = false;

	if (&to == sentinel)
	{
		t = last();
		hasto = true;
		includeto = true;
	}
	else
	{
		if (from == to)
		{
			return new TreeSetI<T>();
		}
	}

	Iterator<T> fromleft = iterator();
	while (fromleft->hasNext())
	{
		T& current = fromleft->next();
		if (current >= from) 
		{
			f = current;
			hasfrom = true;
			break;
		}
	}
	if (!hasfrom) 
	{
		return new TreeSetI<T>();
	}

	if (!hasto)
	{
		Iterator<T> fromright = iterator();
		while (fromright->hasNext())
		{
			T& current = fromright->next();
			if (current >= to) 
			{
				t = current;
				hasto = true;
				break;
			}
		}
	}

	return new TreeSetFromToI<T>(f, t, true, hasto, includeto, this);
}

//********************************************************************

template< typename T > 
SortedSet<T> TreeSetI<T>::tailSet(const T& from) throw (IndexOutOfBoundsException) 
{
	return subSet(from, *sentinel);

}

//********************************************************************

template< typename T >
void TreeSetI<T>::add__(const T& t, 
					   TreeNode<T>*& p, 
					   TreeNode<T>* up_, 
					   int& grown) 
{
	TreeNode<T> *p1;
	TreeNode<T> *p2;
	if (!p) 
	{
		elements++;
		p = new TreeNode<T>(t, up_);
		grown = !0;
	}
	else
	{
		int c = 0 == comp.get() ? p->data < t ? -1 : p->data == t ? 0 : 1 : comp->compare(p->data, t);

		if (0 < c) 
		{
			add__(t, p->left, p, grown);
			if (grown) 
			{
				switch (p->balance) 
				{
				case  1: p->balance = 0; grown = 0; break;
				case  0: p->balance = -1; break;
				case -1: p1 = p->left;
					if (p1->balance == -1) 
					{
						p->left = p1->right;
						if (p->left)            
							p->left->up = p;    
						p1->right = p;
						p1->up = p->up;         
						p->up = p1;             
						p->balance = 0;
						p = p1;
					}
					else 
					{
						p2 = p1->right;
						p1->right = p2->left;
						if (p1->right)          
							p1->right->up = p1; 
						p2->left = p1;
						if (p2->left)           
							p2->left->up = p2;  
						p->left = p2->right;
						if (p2->right)          
							p2->right->up = p;  
						p2->right = p;
						p2->up = p->up;         
						p->up = p2;             
						if (p2->balance == -1)
							p->balance = 1;
						else
							p->balance = 0;
						if (p2->balance == 1)
							p1->balance = -1;
						else
							p1->balance = 0;
						p = p2;
					}
					p->balance = 0; grown =  0;
					break;
				} // switch
			} // if
		} // if
		else
		{
			if (0 > c) 
			{                     
				add__(t, p->right, p, grown);
				if (grown) 
				{
					switch (p->balance) 
					{
					case -1: p->balance = 0; grown = 0; break;
					case  0: p->balance = 1; break;
					case  1: p1 = p->right;
						if (p1->balance == 1) 
						{
							p->right = p1->left;
							if (p->right)                
								p->right->up = p;        
							p1->left = p;
							p1->up = p->up;              
							p->up = p1;                  
							p->balance = 0;
							p = p1;
						}
						else 
						{
							p2 = p1->left;
							p1->left = p2->right;
							if (p1->left)                
								p1->left->up = p1;       
							p2->right = p1;
							if (p2->right)               
								p2->right->up = p2;      
							p->right = p2->left;
							if (p2->left)                
								p2->left->up = p;        
							p2->left = p;
							p2->up = p->up;              
							p->up = p2;                  
							if (p2->balance == 1)
								p->balance = -1;
							else
								p->balance = 0;
							if (p2->balance == -1)
								p1->balance = 1;
							else
								p1->balance = 0;
							p = p2;
						}
						p->balance = 0; grown =  0;
						break;
					} // switch
				} // if
			} // if
			else 
			{ // replacing with t
				p->data = t;
			}
		} // else
	} // else
}

//********************************************************************

template< typename T >
void TreeSetI<T>::remove__(const T& t, 
						  TreeNode<T>*& p, 
						  int& shrunk, 
						  int& removed) 
{
	TreeNode<T> *q;
	if (p)
	{
		int c = 0 == comp.get() ? p->data < t ? -1 : p->data == t ? 0 : 1 : comp->compare(p->data, t);

		if (0 < c) 
		{
			remove__(t, p->left, shrunk, removed);
			if (shrunk)
				balanceleft__(p, shrunk);
		}
		else
		{
			if (0 > c) 
			{
				remove__(t, p->right, shrunk, removed);
				if (shrunk)
					balanceright__(p, shrunk);
			}
			else 
			{
				q = p;
				if (!q->right) 
				{	
					p = q->left;
					if (p)                  
						p->up = q->up;      
					shrunk = !0;
					q->left = q->right = 0;
					delete q; 
					elements--;
					removed = !0;
				}
				else
				{
					if (!q->left) 
					{
						p = q->right;
						if (p)               
							p->up = q->up;   
						shrunk = !0;
					}
					else 
					{
						remove___(q->left, q, shrunk);
						if (shrunk)
							balanceleft__(p, shrunk);
					}
					q->left = q->right = 0;
					delete q; 
					elements--;
					removed = !0;
				}
			}
		}
	}
}

//********************************************************************

template< typename T >
void TreeSetI<T>::balanceleft__(TreeNode<T>*& p, 
							   int& shrunk) 
{
	TreeNode<T> *p1;
	TreeNode<T> *p2;
	int b1, b2;
	switch (p->balance) 
	{
	case -1: p->balance = 0; break;
	case  0: p->balance = 1; shrunk = 0; break;
	case  1: p1 = p->right;
		b1 = p1->balance;
		if (b1 >= 0) 
		{
			p1->up = p->up;  
			p->right = p1->left;
			if (p->right)
				p->right->up = p;      
			p1->left = p;
			if (p1->left)
				p1->left->up = p1;     
			if (!b1) 
			{
				p->balance = 1;
				p1->balance = -1;
				shrunk = 0;
			}
			else 
			{
				p->balance = 0;
				p1->balance = 0;
			}
			p = p1;
		}
		else 
		{
			p2 = p1->left;
			p2->up = p->up;  
			b2 = p2->balance;
			p1->left = p2->right;
			if (p1->left)
				p1->left->up = p1;     
			p2->right = p1;
			if (p2->right)
				p2->right->up = p2;    
			p->right = p2->left;
			if (p->right)
				p->right->up = p;      
			p2->left = p;
			if (p2->left)
				p2->left->up = p2;     
			if (b2 == 1)
				p->balance = -1;
			else
				p->balance = 0;
			if (b2 == -1)
				p1->balance = 1;
			else
				p1->balance = 0;
			p = p2;
			p2->balance = 0;
		}
		break;
	} // switch
}

//********************************************************************

template< typename T >
void TreeSetI<T>::balanceright__(TreeNode<T>*& p, 
								int& shrunk) 
{
	TreeNode<T> *p1;
	TreeNode<T> *p2;
	int b1, b2;
	switch (p->balance) 
	{
	case  1: p->balance = 0; break;
	case  0: p->balance = -1; shrunk = 0; break;
	case -1: p1 = p->left;
		b1 = p1->balance;
		if (b1 <= 0) 
		{
			p1->up = p->up;      
			p->left = p1->right;
			if (p->left)
				p->left->up = p;        
			p1->right = p;
			if (p1->right)
				p1->right->up = p1;     
			if (!b1) 
			{
				p->balance = -1;
				p1->balance = 1;
				shrunk = 0;
			}
			else 
			{
				p->balance = 0;
				p1->balance = 0;
			}
			p = p1;
		}
		else 
		{
			p2 = p1->right;
			p2->up = p->up;   
			b2 = p2->balance;
			p1->right = p2->left;
			if (p1->right)
				p1->right->up = p1;     
			p2->left = p1;
			if (p2->left)
				p2->left->up = p2;      
			p->left = p2->right;
			if (p->left)
				p->left->up = p;        
			p2->right = p;
			if (p2->right)
				p2->right->up = p2;     
			if (b2 == -1)
				p->balance = 1;
			else
				p->balance = 0;
			if (b2 == 1)
				p1->balance = -1;
			else
				p1->balance = 0;
			p = p2;
			p2->balance = 0;
		}
		break;
	} // switch
}

//********************************************************************

template< typename T >
void TreeSetI<T>::remove___(TreeNode<T>*& p, 
						   TreeNode<T>*& q, 
						   int& shrunk) 
{
	if (p->right) 
	{
		remove___(p->right, q, shrunk);
		if (shrunk)
			balanceright__(p, shrunk);
	}
	else 
	{
		q->data = p->data; // :TODO: need to improve algorithm
		q = p;
		p = p->left;
		if (p) p->up = q->up;     
		shrunk = !0;
	}
}

//********************************************************************

template< typename T >
TreeNode<T> *TreeSetI<T>::contains__(const T& t, TreeNode<T>* p) const 
{
	if (p)
	{
		int c = 0 == comp.get() ? p->data < t ? -1 : p->data == t ? 0 : 1 : comp->compare(p->data, t);

		if (0 < c) 
			return contains__(t, p->left);
		else
		{
			if (0 > c) 
				return contains__(t, p->right);
			else
				return p;
		}
	}
	return 0;
}

//********************************************************************

template< typename T >
T& TreeSetI<T>::lookup(const T& t) const 
{
	return lookup__(t, root);
}

//********************************************************************

template< typename T >
T& TreeSetI<T>::lookup__(const T& t, TreeNode<T>* p) const 
{
	if (p)
	{
		int c = 0 == comp.get() ? p->data < t ? -1 : p->data == t ? 0 : 1 : comp->compare(p->data, t);

		if (0 < c) 
			return lookup__(t, p->left);
		else
		{
			if (0 > c) 
				return lookup__(t, p->right);
			else 
			{
				return p->data;
			}
		}
	}
	return *((T*)0); // null ref
}

//********************************************************************

template< typename T >
TreeNode<T>* TreeSetI<T>::lookupNode(const T& t) const 
{
	return contains__(t, root);
}

//********************************************************************

template< typename T >
void TreeSetI<T>::preorder__(TreeNode<T>* p, TreeNode<T>*& q) 
{
	if (p) 
	{
		q = new TreeNode<T>(p->data, p->up, p->balance);
		preorder__(p->left, q->left);
		preorder__(p->right, q->right);
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
