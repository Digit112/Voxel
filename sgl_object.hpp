#ifndef sgl_object
#define sgl_object

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
		
		// TODO change to union
		mesh_wire m;
		
		object();
		
		void translate(vecd3 t);
		void rotate(quaternion r, bool is_global);
		void rotate(vecd3 axis, double theta, bool is_global);
		void scale(vecd3 s);
		
		mesh_wire& applied(mesh_wire& o) const;
	};
}

#include "sgl_object.cpp"

#endif
