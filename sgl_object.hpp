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
		
		void translate(vecd3 t);
		void rotate(quaternion r);
		void scale(vecd3 s);
	};
}

#include "sgl_object.cpp"

#endif
