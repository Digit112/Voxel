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
		mesh_wire(const mesh_wire&);
		mesh_wire(int pn, int en);
		mesh_wire(vecd3* p, int pn, veci2* e, int en);
		
		mesh_wire(unsigned short primitive, int opt);
		
		void operator=(const mesh_wire&);
		
		~mesh_wire();
		
		static const unsigned short CUBE = 0;
		static const unsigned short CYLINDER = 1;
	};
}

#include "sgl_mesh.cpp"

#endif
