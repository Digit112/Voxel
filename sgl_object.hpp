#ifndef sgl_object
#define sgl_object

namespace sgl {
	class object {
	public:
		// Transformations on this object
		vecd3 p;
		quaternion r;
		vecd3 s;
		
		// This object's mesh
		wire_mesh m;
		
		// Render this object. Called by parent.
		RGBAD (*render)(app_handle* ah, object* o, vecd3 pos, vecd3 dir, void* state, double dt);
		
		// This object's children.
		darray<object*> children;
		
		// This object's parent
		object* parent;
		
		// RGBA
		unsigned int color;
		
		bool is_hidden;
		
		// Default
		object();
		
		// Initialize with space for child objects
		object(int n);
		
		// Set or change this object's parent
		void set_parent(object* p);
		
		void translate(vecd3 t, bool is_global);
		
		void rotate(quaternion r, bool is_global);
		void rotate(vecd3 offset, quaternion r, bool is_global);
		void rotate(vecd3 axis, double theta, bool is_global);
		void rotate(vecd3 offset, vecd3 axis, double theta, bool is_global);
		
		void scale(vecd3 s);
		void scale(vecd3 offset, vecd3 s);
		
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
