#ifndef sgl_mesh
#define sgl_mesh

#include "string.h"

namespace sgl {
	class wire_mesh;
	
	class wire_mesh {
	public:
		darray<vecd3> p;
		darray<veci2> e;
		
		enum primtype : int {CUBE, POLYGON, GRID, ICOSAHEDRON, CYLINDER, UVSPHERE, TORUS};
		
		wire_mesh();
		wire_mesh(int pn, int en);
		wire_mesh(darray<vecd3> p, darray<veci2> e);
		
		// From file
		wire_mesh(char*);
		
		void translate(vecd3 t);
		void rotate(quaternion r);
		void rotate(vecd3 axis, double theta);
		void scale(vecd3 s);
		
		wire_mesh(primtype prim, int opt1, int opt2, double opt3);
	};
}

#include "sgl_mesh.cpp"

#endif
