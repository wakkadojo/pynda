#ifndef UTILS_H
#define UTILS_H

#include <cassert>
#include <iostream>

template <class T> struct node
{
	T data;
	node<T> * next, * last; // last as in PREVIOUS
	node () { next = last = 0; }
	node (T data) { next = last = 0; this->data = data; }
};

// Linked list
// 

// A linked list with special functionality intended for simulation.
// Specifically, the ability to retrieve the pointer of the data in the
// linked list allowing for O(1) lookups after identifying collisions.
// TODO: replace asserts with exceptions
template <class T> class list
{
	node<T> * top, * cursor;
	int n;
	private:
		bool set_cursor (int);
	public:
		list () { top = 0; n = 0; seek (); }
		~list () { clear (); }
		int length () { return n; }
		bool is_empty () { return top == 0; } // or could do n == 0
		bool at_end () { return cursor == 0; } 
		bool next ();
		void seek () { cursor = top; }
		void insert (T);
		void clear ();
		bool remove (); // remove the node the cursor is on and decrement
		bool remove (int i) { set_cursor (i); return remove (); }
		//T * get_p () { assert (!at_end ()); return &(cursor->data); }
		T & get () { assert (!at_end ()); return cursor->data; }
		//T * get_p (int i) { set_cursor (i); return get_p (); }
		T & get (int i) { set_cursor (i); return get (); }
};

template <class T> bool list<T>::next ()
{
	if (!at_end ())
	{
		cursor = cursor->next;
		return true;
	}
	return false;
}

template <class T> bool list<T>::set_cursor (int i)
{
	seek ();
	bool result = true; // so that if i == 0 and 0 exists, result is true
	if (i > n)
		result = false;
	else 
		for (int j=0; j<i; ++j)
			result = next ();
	return result;
}

template <class T> void list<T>::insert (T data)
{
	++n;
	node<T> * new_top = new node<T> (data);
	new_top->next = top;
	if (!is_empty ())
		top->last = new_top;
	top = new_top;
	seek ();
}

template <class T> bool list<T>::remove ()
{
	// bump back if somewhere cursor in the middle, bump forward along with top
	// if cursor is at top
	if (!at_end ())
	{
		--n;
		node<T> * temp = cursor;
		if (cursor->next) // same whether or not cursor == top
			cursor->next->last = cursor->last; 
		if (cursor != top)
		{
			cursor->last->next = cursor->next;
			cursor = cursor->last;
		}
		else
			top = cursor = cursor->next;
		delete temp;
		return true;
	} else {
		// because this method is a little funky i include a little extra
		// explanation for a false return than other methods for easier
		// debugging
		std::cerr << "ERROR: cannot remove from null end of list" << std::endl;
		return false;
	}
}

template <class T> void list<T>::clear ()
{
	seek ();
	while (!is_empty ())
		remove ();
}

// End linked list
//

// Stack
//

template <class T> class stack
{
	node<T> * top;
	public:
		stack () { top = 0; }
		~stack() { clear (); }
		bool is_empty () { return top == 0; }
		void clear () { while (!is_empty ()) { pop (); } }
		void push (T);
		T pop ();
};

template <class T> void stack<T>::push (T data)
{
	node<T> * new_top = new node<T> (data);
	new_top->next = top;
	top = new_top;
}

template <class T> T stack<T>::pop ()
{
	assert (!is_empty ());
	node<T> * temp = top;
	top = top->next;
	T ret_data = temp->data;
	delete temp;
	return ret_data;
}

// End stack
//

#endif // UTILS_H
