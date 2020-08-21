template<class T>
darray<T>::darray() : data(NULL), size(0), cap(0) {}

template<class T>
darray<T>::darray(int s) : size(s), cap(s) {
	data = new T[s];
}

template<class T>
darray<T>::darray(int s, int c) : size(s), cap(c) {
	if (s > c) {
		printf("Error: darray initialized with greater size than capacity.\n");
	}
	data = new T[c];
}

template<class T>
darray<T>::darray(const darray<T>& a) : size(a.size), cap(a.cap) {
	if (cap > 0) {
		data = new T[cap];
		for (int i = 0; i < size; i++) {
			data[i] = a.data[i];
		}
	}
}

template<class T>
void darray<T>::operator=(const darray<T>& a) {
	if (data != NULL) {
		delete[] data;
		data = NULL;
	}
	size = a.size;
	cap = a.cap;
	data = new T[cap];
	for (int i = 0; i < size; i++) {
		data[i] = a.data[i];
	}
}

template<class T>
T& darray<T>::operator()(int i) {
	if (i < 0 && -i <= size) {
		return data[size+i];
	}
	if (i >= 0 && i < size) {
		return data[i];
	}
	printf("Error: Attempt to access darray of size %d at invalid index %d.\n", size, i);
	return data[0];
}

template<class T>
inline T& darray<T>::operator[](int i) {
	return data[i];
}

template<class T>
void darray<T>::initialize(T def) {
	for (int i = 0; i < size; i++) {
		data[i] = def;
	}
}

template<class T>
void darray<T>::set_size(int s) {
	if (s == 0) {
		size = 0;
		return;
	}
	if (s > cap) {
		cap = s;
		if (data != NULL) {
			int minsize = s > size ? size : s;
			T* temp = data;
			data = new T[cap];
			for (int i = 0; i < minsize; i++) {
				data[i] = temp[i];
			}
			delete[] temp;
		} else {
			data = new T[cap];
		}
	}
	size = s;
}

template<class T>
void darray<T>::set_cap(int c) {
	if (c == 0) {
		size = 0;
		cap = 0;
		if (data != NULL) {
			delete[] data;
			data = NULL;
		}
		return;
	}
	cap = c;
	if (data != NULL) {
		int minsize = size > c ? c : size;
		if (size > cap) {
			size = cap;
		}
		T* temp = data;
		data = new T[cap];
		for (int i = 0; i < minsize; i++) {
			data[i] = temp[i];
		}
		delete[] temp;
	} else {
		data = new T[cap];
	}
}

template<class T>
darray<T>::~darray() {
	if (data != NULL) {
		delete[] data;
	}
}


