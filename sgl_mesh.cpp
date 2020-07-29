namespace sgl {
	mesh_wire::mesh_wire() : p(NULL), pn(0), e(NULL), en(0) {}
	mesh_wire::mesh_wire(const mesh_wire& m) : pn(m.pn), en(m.en) {
		p = new vecd3[pn];
		e = new veci2[en];
		
		for (int i = 0; i < pn; i++) {
			m.p[i] = p[i];
		}
		for (int i = 0; i < en; i++) {
			m.e[i] = e[i];
		}
	}
	mesh_wire::mesh_wire(int pn, int en) : pn(pn), en(en) {
		p = new vecd3[pn];
		e = new veci2[en];
	}
	mesh_wire::mesh_wire(vecd3* p, int pn, veci2* e, int en) : p(p), pn(pn), e(e), en(en) {}
	
	mesh_wire::mesh_wire(unsigned short primitive, int opt) {
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
		else if (primitive == 1 && opt > 2) {
			pn = opt * 2;
			en = opt * 3;
			
			p = new vecd3[pn];
			e = new veci2[en];
			
			double x, y;
			p[0] = vecd3(0, 1, 1);
			p[1] = vecd3(0, 1, -1);
			e[0] = veci2(0, 1);
			e[1] = veci2(0, 2*opt-2);
			e[2] = veci2(1, 2*opt-1);
			for (int i = 1; i < opt; i++) {
				y = cos(i/(float) opt * 6.28318);
				x = sin(i/(float) opt * 6.28318);
				
				p[i*2] = vecd3(x, y, 1);
				p[i*2+1] = vecd3(x, y, -1);
				
				e[i*3]   = veci2(i*2,   i*2+1);
				e[i*3+1] = veci2(i*2,   i*2-2);
				e[i*3+2] = veci2(i*2+1, i*2-1);
			}
		}
	}
	
	void mesh_wire::operator=(const mesh_wire& a) {
		pn = a.pn;
		en = a.en;
		
		p = new vecd3[pn];
		e = new veci2[en];
		
		for (int i = 0; i < a.pn; i++) {
			p[i] = a.p[i];
		}
		for (int i = 0; i < a.en; i++) {
			e[i] = a.e[i];
		}
	}
	
	mesh_wire::~mesh_wire() {
		if (p != NULL) {
			delete[] p;
		}
		if (e != NULL) {
			delete[] e;
		}
	}
}
