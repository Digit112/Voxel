#ifndef sgl_object
#define sgl_object

#include "darray.hpp"
#include "sgl_mesh.hpp"
#include "sgl_vec.hpp"

namespace sgl {
	class object {
	public:
		// Transformations on this object
		vecd3 p;
		quaternion r;
		vecd3 s;
		
		// This object's mesh
		wire_mesh m;
		
		// How to render this object
		unsigned short render_function = 0;
		
		// "render_function = 0"
		// CPU render function called per-object
		void (*raster_method)(app_handle*, cam*, object*, void*, double) =  wireframe_render;
		
		// "render_function = 1"
		// GPU render function called per-object
		// Although they are not specified here, this must have the same arguments as raster_method.
		cl_kernel* raster_kernel = NULL;
		
		// "render_function = 2"
		// CPU render function called per-pixel
		void (*raytrace_method)(app_handle*, cam*, object*, void*, double) = NULL;
		
		// "render_function = 3"
		// GPU render function called per-pixel
		// This must have the same arguments as raytrace_method.
		cl_kernel* raytrace_kernel = NULL;
		
		// This object's children.
		darray<object*> children;
		
		// This object's parent
		object* parent;
		
		unsigned int color;
		
		bool is_hidden;
		
		// Default
		object();
		
		// Initialize with space for child objects
		object(int);
		
		// Properly set or change this object's parent
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
