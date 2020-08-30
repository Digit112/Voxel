template<class T>
dmatrix<T>::dmatrix() : data(NULL), capx(0), capy(0), sizex(0), sizey(0) {}

template<class T>
dmatrix<T>::dmatrix(int sx, int sy) : capx(sx), capy(sy), sizex(sx), sizey(sy) {
	data = new T[sx*sy];
}

template<class T>
dmatrix<T>::dmatrix(int sx, int sy, int cx, int cy) : capx(cx), capy(cy), sizex(sx), sizey(sy) {
	data = new T[cx*cy];
}

template<class T>
void dmatrix<T>::initialize(T def) {
	for (int x = 0; x < sizex; x++) {
		for (int y = 0; y < sizey; y++) {
			data[y*capx+x] = def;
		}
	}
}

template<class T>
T* dmatrix<T>::operator[](int i) {
	return data + sizex*i;
}

template<class T>
dmatrix<T> dmatrix<T>::operator*(const dmatrix<T>& a) {
	if (sizex != a.sizey) {
		printf("Attempted to multiply two dmatrix objects where the width of the first matrix does not equal the height of the second matrix. If your matrices use column-major ordering, try swapping the order of operands.\n");
	}
	
	dmatrix<T> out(a.sizex, sizey);
	
	for (int x = 0; x < a.sizex; x++) {
		for (int y = 0; y < sizey; y++) {
			
}

template<class T>
dmatrix<T>::~dmatrix() {
	if (data != NULL) {
		delete[] data;
	}
}
