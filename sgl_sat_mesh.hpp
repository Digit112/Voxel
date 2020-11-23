#ifndef sgl_sat_mesh
#define sgl_sat_mesh

// This mesh type is a huge improvement on the previous wire_mesh data type. It is a highly flexible and permissive data type for storing all manner of meshes.
// This data type does not require that meshes be manifold, orientable, or contiguous. Points may exist without edges and edges may exist without faces.
// Each vertex record contains its world coordinates and a list of connected edges.
// Each edge record contains its endpoints and a list of connected faces.
// Each face record contains a list of edges and vertices and the UV Coordinates for each vertex.

namespace sgl {
	namespace sat {
		class rec_v {
		public:
			vecd3 p;
			rec_e* e;
			
			rec_v();
			~rec_v();
		};
		
		class rec_e {
		public:
			rec_v* a;
			rec_v* b;
			
			rec_f* f;
			
			rec_e();
			~rec_e();
		};
		
		class rec_f {
		public:
			vecd2* uv;
			rec_v* p;
			rec_e* e;
			
			rec_f();
			~rec_f();
		};
	}
	
	class sat_mesh {
		rec_v* v;
		rec_e* e;
		rec_f* f;
		
		enum primtype : int {CUBE};
		
		sat_mesh();
		~sat_mesh();
	};
}

#include "sgl_sat_mesh.cpp"

#endif
