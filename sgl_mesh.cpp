namespace sgl {
	wire_mesh::wire_mesh() : p(0), e(0) {}
	
	wire_mesh::wire_mesh(int pn, int en) : p(pn), e(en) {}
	wire_mesh::wire_mesh(darray<vecd3> p, darray<veci2> e) : p(p), e(e) {}
	
	wire_mesh::wire_mesh(char* fn) {
		FILE* fp;
		
		int a;
		int b;
		
		p.size = 0;
		e.size = 0;
		
		int pc = 0;
		int ec = 0;
		
		fp = fopen(fn, "r");
		if (fp == NULL) {
			printf("Could not find .v3d file: \"%s\"\n", fn);
			return;
		}
		
		char str[128];
		char substr[128];
		
		bool pn_known = false;
		bool en_known = false;
		
		while(fgets(str, 128, fp) != NULL) {
			if (str[0] == '\n' || str[0] == '\t' || str[0] == ' ' || str[0] == '#') {
				continue;
			}
			
			if (str[0] == 'p' && str[1] == 'n') {
				next(str + 2, 0, substr);
				p.size = atoi(substr);
				p = darray<vecd3>(p.size);
				pn_known = true;
			}
			else if (str[0] == 'v') {
				if (!pn_known) {
					printf("Attempted to declare vertex prior to vertex count in v3d file \"%s\"\n", fn);
					return;
				}
				
				a = next(str + 1, 0, substr);
				p[pc].x = atof(substr);
				
				a = next(str + 1, a+1, substr);
				p[pc].y = atof(substr);
				
				next(str + 1, a+1, substr);
				p[pc].z = atof(substr);
				
				pc++;
			}
			
			if (str[0] == 'e' && str[1] == 'n') {
				next(str + 2, 0, substr);
				e.size = atoi(substr);
				e = darray<veci2>(e.size);
				en_known = true;
			}
			else if (str[0] == 'e') {
				if (!en_known) {
					printf("Attempted to declare edge prior to edge count in v3d file \"%s\"\n", fn);
					return;
				}
				
				a = next(str + 1, 0, substr);
				e[ec].x = atof(substr);
				
				next(str + 1, a+1, substr);
				e[ec].y = atof(substr);
				
				ec++;
			}
		}
		
		fclose(fp);
	}
	
	wire_mesh::wire_mesh(primtype prim, int opt1 = 0, int opt2 = 0, double opt3 = 0.0) {
		if (prim == CUBE) {
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
			p = darray<vecd3>(opt2);
			e = darray<veci2>(opt2);
			
			double theta;
			for (int i = 0; i < p.size; i++) {
				theta = (double) i/p.size * 6.28318;
				p[i] = vecd3(cos(theta), sin(theta), 0);
				e[i] = veci2(i, i-1);
			}
			e[0] = veci2(0, p.size-1);
		}
		else if (prim == GRID) {
			p = darray<vecd3>(opt1*opt2);
			e = darray<veci2>((opt1-1)*opt2 + (opt2-1)*opt1);
			
			double xp;
			double yp;
			for (int x = 0; x < opt1; x++) {
				xp = x / (opt1-1.0) * 2 - 1;
				for (int y = 0; y < opt2; y++) {
					yp = y / (opt2-1.0) * 2 - 1;
					p[x+y*opt1] = vecd3(xp, yp, 0);
					
					if (x < opt1 - 1) {
						e[x + y*(opt1+(opt1-1))] = veci2(x+y*opt1, x+y*opt1+1);
					}
					if (y < opt2 - 1) {
						e[x + y*(opt1+(opt1-1)) + (opt1-1)] = veci2(x+y*opt1, x+(y+1)*opt1);
					}
				}
			}
		}
		else if (prim == ICOSAHEDRON) {
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
				int opn = p.size;
				int oen = e.size;
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
				
				p.size = e.size * (i-1) + p.size;
				e.size = 30 * pow(pow(2, i-1), 2);
				
//				delete[] p;
//				delete[] e;
				p = darray<vecd3>(p.size);
				e = darray<veci2>(e.size);
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
			p = darray<vecd3>(opt1 * 2);
			e = darray<veci2>(opt1 * 3);
			
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
			p = darray<vecd3>(opt2 * opt1 + 2);
			e = darray<veci2>(p.size*2 + opt1 * 3);
			
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
			p[p.size-2] = vecd3(0, 0, 1);
			p[p.size-1] = vecd3(0, 0, -1);
			for (int r = 0; r < opt1; r++) {
				e[e.size-2*(r+1)-1] = veci2(p.size-1, opt2*r+opt2-1);
				e[e.size-2*(r+1)  ] = veci2(p.size-2, opt2*r);
			}
		}
		else if (prim == TORUS && opt1 > 2 && opt2 > 2) {
			p = darray<vecd3>(opt1 * opt2);
			e = darray<veci2>(p.size*2);
			
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
		else {
			printf("Failed to create mesh of type %d and options %d, %d, %f.\n", prim, opt1, opt2, opt3);
		}
	}
	
	void wire_mesh::translate(vecd3 t) {
		for (int i = 0; i < p.size; i++) {
			p[i] = p[i] + t;
		}
	}
	
	void wire_mesh::rotate(quaternion r) {
		for (int i = 0; i < p.size; i++) {
			p[i] = r.apply(p[i]);
		}
	}
	
	void wire_mesh::rotate(vecd3 axis, double theta) {
		quaternion r = quaternion(axis, theta);
		for (int i = 0; i < p.size; i++) {
			p[i] = r.apply(p[i]);
		}
	}
	
	void wire_mesh::scale(vecd3 s) {
		for (int i = 0; i < p.size; i++) {
			p[i] = p[i] * s;
		}
	}
}











