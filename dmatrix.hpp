#ifndef util_dmatrix
#define util_dmatrix

/*
	The dmatrix class stores a rectangular, 2-dimensional dynamically allocated matrix. All memory allocation and deallocation is handled internally.
	A dmatrix has both a capacity (shortened to "cap") and a size, each of which have an x and y variant (capx, capy, sizex, and sizey)
	The capacity represents the allocated storage space and therefore a change in capacity of either dimension performs a reallocation even if the capacity is decreased. The amount of memory currently alloc'd is calculated with capx*capy*sizeof(T).
	size is a purely logical value which can be changed without performing a reallocation or copying any values.
	Changing either the capacity or size will not ever influence the position of values, but data may be deleted if the matrix shrinks.
*/

// 2-dimensional matrix.
template<class T>
class dmatrix {
public:
	T* data;
	int capx;
	int capy;
	
	int sizex;
	int sizey;
	
	dmatrix<T>();
	
	dmatrix<T>(int sx, int sy);
	dmatrix<T>(int sx, int sy, int cx, int cy);
	
	// Returns pointer to relevant row.
	T* operator[](int i);
	// identical to [], except with Fortran-like syntax ([,] instead of [][]) and supports bound checking and negative indices.
//	T& operator()(int x, int y);
	
	// Matrix multiplication
	dmatrix<T> operator*(const dmatrix<T>&);
	
	// Set all values to "def"
	void initialize(T def);
	
	~dmatrix();
};

#include "dmatrix.cpp"

#endif
