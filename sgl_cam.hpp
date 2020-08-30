#ifndef sgl_cam
#define sgl_cam

#include "sgl_vec.hpp"
#include "sgl_object.hpp"

namespace sgl {
	class cam {
	public:
		vecd3 p;
		quaternion r;
		
		cam();
		
		void translate(vecd3 t, bool is_global);
		void rotate(quaternion r, bool is_global);
		void rotate(vecd3 axis, double theta, bool is_global);
		
		// Apply the inverse camera transform to the given object and returns the result of that tranformation.
		object apply(const object&);
		
		// Apply the inverse camera transform to the given mesh.
		void apply(mesh_wire&);
	};
}

#include "sgl_cam.cpp"

#endif
