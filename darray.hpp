/*
	The darray class stores a 1-dimensional dynamically allocated array. All memory allocation and deallocation is handled internally.
	A darray has a capacity which represents the amount of elements that can fit in the allocated space, and a size
	which can be changed to any value below the cap without performing a reallocation, but which is otherwise the same as capacity.
	The capacity and size can be set independently. Any change in the capacity will perform a reallocation, even a decrease in capacity.
	Setting the capacity to 0 will free all allocated memory belonging to the object.
	
	The copy constructor and assignment operator both call the copy constructor of each element in the list.
	
	Increasing the size to beyond the capacity will increase the cap to match the size and perform a reallocation.
	Decreasing the cap to below the size will decrease the size to match in addition to performing the reallocation.
	
	Automatic changes in capacity are usually very inefficient. Changes in capacity should all be performed manually via a call to set_cap()
*/

#ifndef util_darray
#define util_darray

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

template<class T>
class darray {
public:
	T* data;	// Pointer to the first element of the array.
	int cap;  // Maximum size that can be used without reallocation.
	int size; // Current logical size of the array. Must be <= cap.
	
	darray<T>();
	darray<T>(int s);
	darray<T>(int s, int c);
	
	darray<T>(const darray<T>&);
	
	void operator=(const darray<T>&);
	
	// Access element at index. Supports negative indices to index from back and bound checking.
	T& operator()(int i);
	// Access element at index. Performs no bound checking and does not support negative indices.
	inline T& operator[](int i);
	// Access element at index. Performs no bound checking and does not support negative indices.
	inline T operator[](int i) const;
	
	// Set all values in the darray to "def"
	void initialize(T def);
	
	// Change the size or the maximum size. If one is set below the other, the other will be reduced as well.
	void set_size(int s);
	void set_cap(int c);
	
	// Add an object to an array
	void append(T);
	
	// Removes the first found instance of this element and memmove's the remaining elements to fill the gap,
	// reduces the size of the array by 1 returns the index of the element removed or -1 if no element was found.
	int remove(T);
	
	~darray();
};

#include "darray.cpp"

#endif
