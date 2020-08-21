#ifndef sgl_wing
#define sgl_wing

#include "sgl_vec.hpp"

namespace sgl {
	namespace wing {
		class rec_v {
		public:
			vecd3 p;
			rec_e* e;
		}
		
		class rec_f {
		public:
			rec_e* e;
		}
		
		class rec_e {
		public:
			rec_v* b; // Beginning vert
			rec_v* e; // Ending vert
			
			rec_f* l; // Left face
			rec_f* r; // Right face
			
			rec_e* ls; // left successor edge
			rec_e* lp; // left predecessor edge
			rec_e* rs; // right successor edge
			rec_e* rp; // right predecessor edge
		}
	}
	
	class wing_mesh {
		wing::rec_v* v;
		wing::rec_e* e;
		wing::rec_f* f;
	}
}

#include "sgl_wing.cpp"

#endif
