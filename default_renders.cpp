// This file just contains some default render functions
namespace sgl {
	void wireframe_render(app_handle* ah, cam* c, object* o, void* state, double dt) {
		sgl::wire_mesh m;
		if (o->is_hidden) {
			return;
		}
		
		// Apply color
		ah->set_foreground(o->color);
		
		// Apply the inverse camera transform to this object, then apply all object transforms to each verticy belonging to it.
		// This allows you to treat the camera as having a default position and rotation.
		c->apply(o->applied(m));
		
		// Convert theta to radians
		double theta_x = c->theta * 3.14159/180;
		double theta_y = theta_x * (double) ah->win_h / ah->win_w;
		
		// Shorthands used for the endpoints of an edge, their difference, and the intersection between this edge and the
		// clipping plane where appropriate.
		vecd3 a; // Projected endpoints of an edge
		vecd3 b;
		vecd3 A; // Unprojected endpoints of an edge
		vecd3 B;
		vecd3 dif;
		double t;
		vecd3 clip_pt;
		
		// Holds the start and end points of the edge prior to render.
		veci2 s;
		veci2 e;
		
		// Cache trig value
		double slope_x = tan(theta_x/2);
		double slope_y = tan(theta_y/2);
		
		// Project each point within the viewing frustum to the cube (-1, -1, -1) (1, 1, 1)
		// Points behind the near clipping plane are skipped,
		// edges including this point are clipped against the near plane while iterating over edges
		darray<vecd3> p_pts(m.pn);
		for (int j = 0; j < m.pn; j++) {
			if (m.p[j].x < c->clip_near) {
				continue;
			}
			
			p_pts[j].x =  (m.p[j].x - c->clip_near) / (c->clip_far - c->clip_near) * 2 - 1;
			p_pts[j].y =  m.p[j].y / (m.p[j].x * slope_x);
			p_pts[j].z = -m.p[j].z / (m.p[j].x * slope_y);
		}
		
		// Iterate over and render each edge.
		for (int j = 0; j < m.en; j++) {
			a = p_pts[m.e[j].x];
			b = p_pts[m.e[j].y];
			
			A = m.p[m.e[j].x];
			B = m.p[m.e[j].y];
			
			// printf("%d: A(%.2f, %.2f, %.2f) B(%.2f, %.2f, %.2f) ", j, A.x, A.y, A.z, B.x, B.y, B.z);
			
			// If both points are behind the near clipping plane or ahead of the far clipping plane, skip this edge.
			if (A.x < c->clip_near && B.x < c->clip_near) {continue;}
			if (A.x > c->clip_far  && B.x > c->clip_far)  {continue;}
			
			// Near-plane clipping. This must occur prior to projection to the cube, as projection prior to near-plane
			// clipping causes errors. Thus, A/B are used instead of a/b
			if (A.x < c->clip_near) {
				dif = B - A;
				// printf("dif(%.2f, %.2f, %.2f) ", dif.x, dif.y, dif.z);
				dif = dif / dif.x * (c->clip_near - A.x) + A;
				// printf("ac(%.2f, %.2f, %.2f) ", dif.x, dif.y, dif.z);
				a.x =  (dif.x - c->clip_near) / (c->clip_far - c->clip_near) * 2 - 1;
				a.y =  dif.y / (dif.x * slope_x);
				a.z = -dif.z / (dif.x * slope_y);
				// printf("ap(%.2f, %.2f, %.2f) bp(%.2f, %.2f, %.2f) ", a.x, a.y, a.z, b.x, b.y, b.z);
			}
			else if (B.x < c->clip_near) {
				dif = A - B;
				// printf("%d: A(%.2f, %.2f, %.2f) B(%.2f, %.2f, %.2f) dif(%.2f, %.2f, %.2f) ", j, A.x, A.y, A.z, B.x, B.y, B.z, dif.x, dif.y, dif.z);
				dif = dif / dif.x * (c->clip_near - B.x) + B;
				// printf("bc(%.2f, %.2f, %.2f) ", dif.x, dif.y, dif.z);
				b.x =  (dif.x - c->clip_near) / (c->clip_far - c->clip_near) * 2 - 1;
				b.y =  dif.y / (dif.x * slope_x);
				b.z = -dif.z / (dif.x * slope_y);
				// printf("bp(%.2f, %.2f, %.2f) ", b.x, b.y, b.z);
			}
			
			// If both points are too far off one side, skip them. Clipping off +/-x is handled above
			if (a.y > 1 && b.y > 1) {
				// printf("clipped off +y\n");
				continue;
			}
			if (a.y < -1 && b.y < -1) {
				// printf("clipped off -y\n");
				continue;
			}
			if (a.z > 1 && b.z > 1) {
				// printf("clipped off +z\n");
				continue;
			}
			if (a.z < -1 && b.z < -1) {
				// printf("clipped off -z\n");
				continue;
			}
			
			// If both points are in view, display this edge as expected.
			if (a.x >= -1 && a.x <= 1 && a.y >= -1 && a.y <= 1 && a.z >= -1 && a.z <= 1 &&
			    b.x >= -1 && b.x <= 1 && b.y >= -1 && b.y <= 1 && b.z >= -1 && b.z <= 1) {
				s.x = (int) ((a.y+1) / 2.0 * ah->win_w);
				s.y = (int) ((a.z+1) / 2.0 * ah->win_h);
				e.x = (int) ((b.y+1) / 2.0 * ah->win_w);
				e.y = (int) ((b.z+1) / 2.0 * ah->win_h);
				ah->draw_line(s.x, s.y, e.x, e.y);
				// printf("in bounds\n");
				continue;
			}
			// printf("\n");
			
			// Clip the edge against the planes.
			dif = b - a;
			// printf("%d: (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f)\n", j, a.x, a.y, a.z, b.x, b.y, b.z);
			if (dif.y != 0) {
				// Clip against +y
				t = 1 / dif.y * (1 - a.y);
				// printf("  +yt: %.2f\n", t);
				if (t > 0 && t < 1) {
					if (a.y > 1) {
						a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
						// printf("    a -> (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
					} else {
						b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
						// printf("    b -> (%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
					}
					dif = b-a;
				}
				// Clip against -y
				t = 1 / dif.y * (-1 - a.y);
				// printf("  -yt: %.2f\n", t);
				if (t > 0 && t < 1) {
					if (a.y < -1) {
						a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
						// printf("    a -> (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
					} else {
						b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
						// printf("    b -> (%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
					}
					dif = b-a;
				}
			}
			if (dif.z != 0) {
				// Clip against +z
				t = 1 / dif.z * (1 - a.z);
				// printf("  +zt: %.2f\n", t);
				if (t > 0 && t < 1) {
					if (a.z > 1) {
						a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
						// printf("    a -> (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
					} else {
						b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
						// printf("    b -> (%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
					}
					dif = b-a;
				}
				// Clip against -z
				t = 1 / dif.z * (-1 - a.z);
				// printf("  -zt: %.2f\n", t);
				if (t > 0 && t < 1) {
					if (a.z < -1) {
						a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
						// printf("    a -> (%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
					} else {
						b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
						// printf("    b -> (%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
					}
				}
			}
			
			s.x = (int) ((a.y+1) / 2.0 * ah->win_w);
			s.y = (int) ((a.z+1) / 2.0 * ah->win_h);
			e.x = (int) ((b.y+1) / 2.0 * ah->win_w);
			e.y = (int) ((b.z+1) / 2.0 * ah->win_h);
			
			ah->draw_line(s.x, s.y, e.x, e.y);
		}
	}
}
