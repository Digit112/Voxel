#ifndef sgl_cam
#define sgl_cam

namespace sgl {
	class cam {
	public:
		vecd3 p;
		quaternion r;
		
		// Camera FOV
		double theta;
		
		// Distance from camera to the near and far clipping planes.
		double clip_near;
		double clip_far;
		
		cam();
		cam(vecd3 p, quaternion r, double theta, double clip_near, double clip_far);
		
		void translate(vecd3 t, bool is_global);
		void rotate(quaternion r, bool is_global);
		void rotate(vecd3 axis, double theta, bool is_global);
		
		// Get this camera's facing direction
		vecd3 forward();
		vecd3 right();
		vecd3 up();
		
		// Apply the inverse camera transform to the given object and returns the result of that tranformation.
		object apply(const object&);
		
		// Apply the inverse camera transform to the given mesh.
		void apply(wire_mesh&);
	};
}

#include "sgl_cam.cpp"

#endif
