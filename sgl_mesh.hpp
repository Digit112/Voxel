#ifndef sgl_mesh
#define sgl_mesh

#include "sgl_vec.hpp"

namespace sgl {
	class mesh_wire;
	
	class mesh_wire {
	public:
		int pn;
		int en;
		
		vecd3* p;
		veci2* e;
		
		enum primtype : int {CUBE, POLYGON, ICOSAHEDRON, CYLINDER, UVSPHERE, TORUS};
		
		mesh_wire();
		mesh_wire(const mesh_wire&);
		mesh_wire(int pn, int en);
		mesh_wire(vecd3* p, int pn, veci2* e, int en);
		
		mesh_wire(primtype prim, int opt1, int opt2, double opt3);
		
		void operator=(const mesh_wire&);
		
		~mesh_wire();
	};
}

#include "sgl_mesh.cpp"

#endif
