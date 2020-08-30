#ifndef sgl_wing
#define sgl_wing

#include "sgl_vec.hpp"
#include "darray.hpp"

namespace sgl {
	namespace wing {
		class rec_v {
		public:
			vecd3 p;
			rec_e* e;
			
			rec_v(vecd3 p, rec_e* e);
		}
		
		class rec_f {
		public:
			rec_e* e;
			
			rec_f(rec_e* e);
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
			
			rec_e(rec_v* b, rec_v* e,
			      rec_f* l, rec_f* r,
			      rec_e* ls, rec_e* lp, rec_e* rs, rec_e* rp);
		}
	}
	
	class wing_mesh {
		darray<wing::rec_v> v;
		darray<wing::rec_e> e;
		darray<wing::rec_f> f;
	}
}

#include "sgl_wing.cpp"

#endif
