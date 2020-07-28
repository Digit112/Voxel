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
		
		mesh_wire();
		mesh_wire(vecd3* p, int pn, veci2* e, int en);
		
		mesh_wire(unsigned short primitive);
		
		~mesh_wire();
		
		static const unsigned short CUBE = 0;
	};
}

#include "sgl_mesh.cpp"

#endif
