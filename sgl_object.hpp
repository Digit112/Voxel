#ifndef sgl_object
#define sgl_object

#include "darray.hpp"
#include "sgl_mesh.hpp"
#include "sgl_vec.hpp"

namespace sgl {
	class object;
	
	class object {
	public:
		// Transformations on this object
		vecd3 p;
		quaternion r;
		vecd3 s;
		
		// This object's mesh
		wire_mesh m;
		
		// This object's children.
		darray<object*> children;
		
		// This object's parent
		object* parent;
		
		unsigned int color;
		
		bool is_hidden;
		
		object();
		
		// Properly set or change this object's parent
		void set_parent(object* p);
		
		void translate(vecd3 t, bool is_global);
		void rotate(quaternion r, bool is_global);
		void rotate(vecd3 axis, double theta, bool is_global);
		void rotate(vecd3 offset, vecd3 axis, double theta, bool is_global);
		void scale(vecd3 s);
		
		vecd3 forward();
		vecd3 right();
		vecd3 up();
		
		void hide();
		void show();
		
		wire_mesh& applied(wire_mesh& o) const;
	};
}

#include "sgl_object.cpp"

#endif
