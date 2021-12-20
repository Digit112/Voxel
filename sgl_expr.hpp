#ifndef sgl_expr
#define sgl_expr

#include <math.h>

namespace sgl {
	// The expr ("expression") class is a binary expression tree node. By combining them, an expression tree can be created.
	// The evaluate function can be used to get a result.
	// If this is a leaf node, value is of type T. Otherwise, it is an operation represented by a function pointer.
	// Many default functions are defined for conveniance below.
	template<class T>
	class expr {
	private:
		union value {
			T v;
			T (*C)(T);
			T (*c)(T, T);
			
			// For quick initialization
			value(T v) : v(v) {}
			value(T (*c)(T)) : C(c) {}
			value(T (*c)(T, T)) : c(c) {}
		};
	
	public:
		// Children
		expr<T>* l;
		expr<T>* r;
		
		// Value or Operation
		value v;
		
		// Constructors
		expr();
		expr(T value);
		expr(T (*unary)(T));
		expr(T (*binary)(T, T));
		
		// Evaluate the expression tree below this node.
		T evaluate();
		
		// Default functions for the purpose of conveniance.
		// Arithmetic
		static T ADD(T a, T b) {return a + b;}
		static T SUB(T a, T b) {return a - b;}
		static T MUL(T a, T b) {return a * b;}
		static T DIV(T a, T b) {return a / b;}
		static T POW(T a, T b) {return pow((double) a, (double) b);}
		static T NEG(T a)      {return -a;}
		static T AVG(T a, T b) {return (a+b)/2;}
		static T MAX(T a, T b) {return a > b ? a : b;}
		static T MIN(T a, T b) {return a < b ? a : b;}
		
		// Comparison
		static T GT (T a, T b) {return a > b;}
		static T LT (T a, T b) {return a < b;}
		static T GTE(T a, T b) {return a >= b;}
		static T LTE(T a, T b) {return a <= b;}
		static T EQ (T a, T b) {return a == b;}
		static T NEQ(T a, T b) {return a != b;}
		
		// Boolean
		static T AND(T a, T b) {return a && b;}
		static T OR (T a, T b) {return a || b;}
		static T NOT(T a)      {return !a;}
		
		// Bitwise
		static T BWA(T a, T b) {return a & b;}
		static T BWO(T a, T b) {return a | b;}
		static T XOR(T a, T b) {return a ^ b;}
		static T INV(T a)      {return ~a;}
	};
}

#include "sgl_expr.cpp"

#endif
