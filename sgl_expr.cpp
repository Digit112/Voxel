namespace sgl {
	// Constructors
	template<class T>
	expr<T>::expr() : v(T()), l(NULL), r(NULL) {}
	
	template<class T>
	expr<T>::expr(T val) : v(val), l(NULL), r(NULL) {}
	
	template<class T>
	expr<T>::expr(T (*custom)(T)) : v(custom), l(NULL), r(NULL) {}
	
	template<class T>
	expr<T>::expr(T (*custom)(T, T)) : v(custom), l(NULL), r(NULL) {}
	
	// Recursively evaluate the expression tree.
	template<class T>
	T expr<T>::evaluate() {
		if (l == NULL) {
			// Base case, return raw value
			return v.v;
		}
		else {
			if (r == NULL) {
				// Unary Operator
				return v.C(l->evaluate());
			}
			else {
				// Binary Operator
				return v.c(l->evaluate(), r->evaluate());
			}
		}
	}
}











