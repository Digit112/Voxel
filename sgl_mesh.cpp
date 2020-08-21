namespace sgl {
	mesh_wire::mesh_wire() : p(), pn(0), e(), en(0) {}
	
	mesh_wire::mesh_wire(int pn, int en) : pn(pn), en(en) {
		p = darray<vecd3>(pn);
		e = darray<veci2>(en);
	}
	mesh_wire::mesh_wire(darray<vecd3> p, int pn, darray<veci2> e, int en) : p(p), pn(pn), e(e), en(en) {}
	
	mesh_wire::mesh_wire(primtype prim, int opt1 = 0, int opt2 = 0, double opt3 = 0.0) {
		if (prim == CUBE) {
			pn = 8;
			en = 12;
			
			p = darray<vecd3>(8);
			e = darray<veci2>(12);
			
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
		else if (prim == POLYGON) {
			pn = opt2;
			en = opt2;
			p = darray<vecd3>(pn);
			e = darray<veci2>(en);
			
			double theta;
			for (int i = 0; i < pn; i++) {
				theta = (double) i/pn * 6.28318;
				p[i] = vecd3(cos(theta), sin(theta), 0);
				e[i] = veci2(i, i-1);
			}
			e[0] = veci2(0, pn-1);
		}
		else if (prim == ICOSAHEDRON) {
			pn = 12;
			en = 30;
			
			p = darray<vecd3>(12);
			e = darray<veci2>(30);
			
			double g = 2 / (1 + sqrt(5));
			
			p[0 ] = vecd3(-1,  g,  0);
			p[1 ] = vecd3( 1,  g,  0);
			p[2 ] = vecd3(-1, -g,  0);
			p[3 ] = vecd3( 1, -g,  0);
			p[4 ] = vecd3( 0, -1,  g);
			p[5 ] = vecd3( 0,  1,  g);
			p[6 ] = vecd3( 0, -1, -g);
			p[7 ] = vecd3( 0,  1, -g);
			p[8 ] = vecd3( g,  0, -1);
			p[9 ] = vecd3( g,  0,  1);
			p[10] = vecd3(-g,  0, -1);
			p[11] = vecd3(-g,  0,  1);
			
			e[0 ] = veci2(0, 2);
			e[1 ] = veci2(0, 5);
			e[2 ] = veci2(0, 7 );
			e[3 ] = veci2(0, 10);
			e[4 ] = veci2(0, 11);
			e[5 ] = veci2(3, 1);
			e[6 ] = veci2(3, 4);
			e[7 ] = veci2(3, 6 );
			e[8 ] = veci2(3, 8 );
			e[9 ] = veci2(3, 9 );
			e[10] = veci2(1, 5 );
			e[11] = veci2(1, 7 );
			e[12] = veci2(1, 9 );
			e[13] = veci2(1, 8 );
			e[14] = veci2(2, 4 );
			e[15] = veci2(2, 6 );
			e[16] = veci2(2, 10);
			e[17] = veci2(2, 11);
			e[18] = veci2(4, 6 );
			e[19] = veci2(4, 9 );
			e[20] = veci2(4, 11);
			e[21] = veci2(7, 8 );
			e[22] = veci2(7, 5 );
			e[23] = veci2(7, 10);
			e[24] = veci2(5, 9 );
			e[25] = veci2(5, 11);
			e[26] = veci2(6, 8 );
			e[27] = veci2(6, 10);
			e[28] = veci2(8, 10);
			e[29] = veci2(9, 11);
			
			// Refine
			for (int i = 2; i <= opt1; i++) {
				int opn = pn;
				int oen = en;
				int le = oen;
				int lp = opn;
				vecd3 op[opn];
				veci2 oe[oen];
				for (int j = 0; j < opn; j++) {
					op[j] = p[j];
				}
				for (int j = 0; j < oen; j++) {
					oe[j] = e[j];
				}
				
				pn = en * (i-1) + pn;
				en = 30 * pow(pow(2, i-1), 2);
				
//				delete[] p;
//				delete[] e;
				p = darray<vecd3>(pn);
				e = darray<veci2>(en);
				for (int j = 0; j < opn; j++) {
					p[j] = op[j];
				}
				for (int j = 0; j < oen; j++) {
					e[j] = oe[j];
				}
				
				for (int j = 0; j < oen; j++) {
					p[lp] = p[e[j].x] + p[e[j].y];
					p[lp] = (p[lp]/2).normalize(1.17557);
				
					e[le] = veci2(lp, e[j].y);
					e[j].y = lp;
				
					lp++;
					le++;
				}
			
				// Create sub-triangles
				for (int a = 0; a < opn; a++) {
					for (int b = a; b < opn; b++) {
						for (int c = b; c < opn; c++) {
							// Check if abc is a triangle
							int ab = -1;
							int bc = -1;
							int ca = -1;
							for (int edge = 0; edge < oen; edge++) {
								if ( (oe[edge].x == a && oe[edge].y == b) || (oe[edge].x == b && oe[edge].y == a) ) {
									ab = edge;
								}
								else if ( (oe[edge].x == b && oe[edge].y == c) || (oe[edge].x == c && oe[edge].y == b) ) {
									bc = edge;
								}
								else if ( (oe[edge].x == c && oe[edge].y == a) || (oe[edge].x == a && oe[edge].y == c) ) {
									ca = edge;
								}
								if (ab != -1 && bc != -1 && ca != -1) {
									break;
								}
							}
							
							if (ab != -1 && bc != -1 && ca != -1) {
								e[le] = veci2(e[ab].y, e[bc].y);
								le++;
								e[le] = veci2(e[bc].y, e[ca].y);
								le++;
								e[le] = veci2(e[ca].y, e[ab].y);
								le++;
							}
						}
					}
				}
			}
		}
		else if (prim == CYLINDER && opt1 > 2) {
			pn = opt1 * 2;
			en = opt1 * 3;
			
			p = darray<vecd3>(pn);
			e = darray<veci2>(en);
			
			double x, y;
			p[0] = vecd3(0, 1, 1);
			p[1] = vecd3(0, 1, -1);
			e[0] = veci2(0, 1);
			e[1] = veci2(0, 2*opt1-2);
			e[2] = veci2(1, 2*opt1-1);
			for (int i = 1; i < opt1; i++) {
				y = cos(i/(float) opt1 * 6.28318);
				x = sin(i/(float) opt1 * 6.28318);
				
				p[i*2] = vecd3(x, y, 1);
				p[i*2+1] = vecd3(x, y, -1);
				
				e[i*3]   = veci2(i*2,   i*2+1);
				e[i*3+1] = veci2(i*2,   i*2-2);
				e[i*3+2] = veci2(i*2+1, i*2-1);
			}
		}
		else if (prim == UVSPHERE && opt1 > 2 && opt2 > 2) {
			pn = opt2 * opt1 + 2;
			en = pn*2 + opt1 * 3;
			
			p = darray<vecd3>(pn);
			e = darray<veci2>(en);
			
			double lr;
			double lh;
			for (int r = 0; r < opt1; r++) {
				lr = (double) r / opt1 * 6.28319;
				for (int h = 0; h < opt2; h++) {
					lh = (double) (h+0.5) / opt2 * 3.14159;
					p[opt2*r+h] = vecd3(sin(lh)*sin(lr), sin(lh)*cos(lr), cos(lh));
				}
			}
			
			// Connect points into squares
			for (int r = 0; r < opt1-1; r++) {
				for (int h = 0; h < opt2-1; h++) {
					e[(opt2*r+h)*2  ] = veci2(opt2*r+h, opt2*r+h+1);
					e[(opt2*r+h)*2+1] = veci2(opt2*r+h, opt2*(r+1)+h);
				}
			}
			
			// Connect last column to first coloumn
			for (int h = 0; h < opt2-1; h++) {
				e[(opt2*(opt1-1)+h)*2  ] = veci2(opt2*(opt1-1)+h, opt2*(opt1-1)+h+1);
				e[(opt2*(opt1-1)+h)*2+1] = veci2(opt2*(opt1-1)+h, h);
			}
			
			// Connect top row
			for (int r = 0; r < opt1-1; r++) {
				e[(opt2*r+opt2-1)*2+1] = veci2(opt2*r+opt2-1, opt2*(r+1)+opt2-1);
			}
			
			// Connect first point to last point
			e[(opt2*(opt1-1)+opt2-1)*2+1] = veci2((opt2*(opt1-1)+opt2-1), opt2-1);
			
			// Build n-gons
			p[pn-2] = vecd3(0, 0, 1);
			p[pn-1] = vecd3(0, 0, -1);
			for (int r = 0; r < opt1; r++) {
				e[en-2*(r+1)-1] = veci2(pn-1, opt2*r+opt2-1);
				e[en-2*(r+1)  ] = veci2(pn-2, opt2*r);
			}
		}
		else if (prim == TORUS && opt1 > 2 && opt2 > 2) {
			pn = opt1 * opt2;
			en = pn*2;
			
			p = darray<vecd3>(pn);
			e = darray<veci2>(en);
			
			double lm;
			double lM;
			for (int m = 0; m < opt2; m++) {
				lm = (double) m / opt2 * 6.28319;
				for (int M = 0; M < opt1; M++) {
					lM = (double) M / opt1 * 6.28319;
					p[m+M*opt2].x = sin(lM) * (1 + sin(lm)*opt3);
					p[m+M*opt2].y = cos(lM) * (1 + sin(lm)*opt3);
					p[m+M*opt2].z = cos(lm) * opt3;
				}
			}
			
			for (int m = 0; m < opt2; m++) {
				for (int M = 0; M < opt1; M++) {
					if (M == opt1-1) {
						e[(m+M*opt2)*2+1] = veci2(m+M*opt2, m);
					} else {
						e[(m+M*opt2)*2+1] = veci2(m+M*opt2, m+(M+1)*opt2);
					}
					
					if (m == opt2-1) {
						e[(m+M*opt2)*2] = veci2(m+M*opt2, M*opt2);
					} else {
						e[(m+M*opt2)*2] = veci2(m+M*opt2, m+M*opt2+1);
					}
				}
			}
		}
	}
}
