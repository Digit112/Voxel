#ifndef sgl_collider
#define sgl_collider

namespace sgl {
	class box_collider {
	public:
		// Distance from the origin of the object this collider is attached to to the positive-x wall (xp), negative-x wall (xn), etc.
		double xp =  1;
		double xn = -1;
		double yp =  1;
		double yn = -1;
		double zp =  1;
		double zn = -1;
		
		// Attempts to collide the given line (defined by an origin and direction) with the walls of this collider and
		// returns the point in global space that the collision occured at.
		// If no collision occurs, returns vecd3(NAN, NAN, NAN)
		// The passed transforms will be applied to the collider prior to raycasting.
		vecd3 raycast(vecd3 p, quaternion r, vecd3 s, vecd3 origin, vecd3 dir);
	};
}

#include "sgl_collider.cpp"

#endif
