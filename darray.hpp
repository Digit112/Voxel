#ifndef util_darray
#define util_darray

#include <stdio.h>
#include <unistd.h>

template<class T>
class darray {
public:
	T* data;
	int cap;
	int size;
	
	darray<T>();
	darray<T>(int s);
	darray<T>(int s, int c);
	
	darray<T>(const darray<T>&);
	
	void operator=(const darray<T>&);
	
	// Access element at location. Supports negative values to index from back and bound checking.
	T& operator[](int i);
	
	// Set all values in the darray to "def"
	void initialize(T def);
	
	// Change the size or the maximum size. If one is set below the other, the other will be reduced as well.
	void set_size(int s);
	void set_cap(int c);
	
	~darray();
};

#include "darray.cpp"

#endif
