namespace sgl {
	mesh_wire::mesh_wire() : p(NULL), pn(0), e(NULL), en(0) {}
	mesh_wire::mesh_wire(vecd3* p, int pn, veci2* e, int en) : p(p), pn(pn), e(e), en(en) {}
	
	mesh_wire::mesh_wire(unsigned short primitive) {
		if (primitive == 0) {
			pn = 8;
			en = 12;
			
			p = new vecd3[8];
			e = new veci2[12];
			
			p[0] = vecd3(-1, -1, -1);
			p[1] = vecd3(-1, -1,  1);
			p[2] = vecd3(-1,  1, -1);
			p[3] = vecd3(-1,  1,  1);
			p[4] = vecd3( 1, -1, -1);
			p[5] = vecd3( 1, -1,  1);
			p[6] = vecd3( 1,  1, -1);
			p[7] = vecd3( 1,  1,  1);
			
			e[0]  = veci2(0, 1);
			e[1]  = veci2(0, 2);
			e[2]  = veci2(0, 4);
			e[3]  = veci2(1, 3);
			e[4]  = veci2(1, 5);
			e[5]  = veci2(2, 3);
			e[6]  = veci2(2, 6);
			e[7]  = veci2(3, 7);
			e[8]  = veci2(4, 5);
			e[9]  = veci2(4, 6);
			e[10] = veci2(5, 7);
			e[11] = veci2(6, 7);
		}
	}
	
	mesh_wire::~mesh_wire() {
		if (p != NULL) {
			delete p;
		}
		if (e != NULL) {
			delete e;
		}
	}
}
