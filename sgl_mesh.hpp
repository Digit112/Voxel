#ifndef sgl_mesh
#define sgl_mesh

#include "sgl_vec.hpp"
#include "darray.hpp"

namespace sgl {
	class mesh_wire;
	
	class mesh_wire {
	public:
		int pn;
		int en;
		
		darray<vecd3> p;
		darray<veci2> e;
		
		enum primtype : int {CUBE, POLYGON, ICOSAHEDRON, CYLINDER, UVSPHERE, TORUS};
		
		mesh_wire();
		mesh_wire(int pn, int en);
		mesh_wire(darray<vecd3> p, int pn, darray<veci2> e, int en);
		
		mesh_wire(primtype prim, int opt1, int opt2, double opt3);
	};
}

#include "sgl_mesh.cpp"

#endif
