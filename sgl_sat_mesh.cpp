namespace sgl {
	namespace sat {
		rec_v::rec_v() : p(0, 0, 0), e(NULL) {}
		rec_e::rec_e() : a(NULL), b(NULL), f(NULL) {}
		rec_f::rec_f() : uv(NULL), p(NULL), e(NULL) {}
		
		rec_v::~rec_v() {if (e  != NULL) free(e) ;}
		rec_e::~rec_e() {if (a  != NULL) free(a) ; if (b != NULL) free(b); if (f != NULL) free(f);}
		rec_f::~rec_f() {if (uv != NULL) free(uv); if (p != NULL) free(p); if (e != NULL) free(e);}
	}
	
	sat_mesh::sat_mesh() : v(NULL), e(NULL), f(NULL) {}
	
	sat_mesh::~sat_mesh() {if (v != NULL) free(v); if (e != NULL) free(e); if (f != NULL) free(f);}
}
