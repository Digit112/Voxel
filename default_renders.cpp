// This file contains the default render functions
namespace sgl {
	// Render function for wire_mesh objects
	// Works with standard consolidation render group.
	// wire_mesh objects with this render function can go directly under the root render group.
	RGBAD wire_mesh_renderer(app_handle* ah, object* o, vecd3 p1, vecd3 d1, void* state, double delta_time) {
//		printf("(%.2lf, %.2lf, %.2lf)\n", d1.x, d1.y, d1.z);
		if (o->is_hidden) {
			return RGBAD(0, 0, 0, 0, INFINITY);
		}
		
//		printf("Ray (%.2lf, %.2lf, %.2lf) + (%.2lf, %.2lf, %.2lf)\n", p1.x, p1.y, p1.z, d1.x, d1.y, d1.z);
		
		for (int i = 0; i < o->m.e.size; i++) {
			veci2 edge = o->m.e[i];
			
			vecd3 l1 = o->m.p[edge.y];
			vecd3 l2 = o->m.p[edge.x];
			
//			printf("Edge %d connects (%.2lf, %.2lf, %.2lf), (%.2lf, %.2lf, %.2lf)\n", i, l1.x, l1.y, l1.z, l2.x, l2.y, l2.z);
			
			vecd3 p2 = l2;
			vecd3 d2 = l1 - p2;
			
//			printf("Connecting Vector is (%.2lf, %.2lf, %.2lf) + (%.2lf, %.2lf, %.2lf)\n", p2.x, p2.y, p2.z, d2.x, d2.y, d2.z);
			
			vecd3 n  = vecd3::cross(d1, d2);
			vecd3 n1 = vecd3::cross(d1, n);
			vecd3 n2 = vecd3::cross(d2, n);
			
//			printf("Intermediary Normals are n: (%.2lf, %.2lf, %.2lf), n1: (%.2lf, %.2lf, %.2lf), n2: (%.2lf, %.2lf, %.2lf)\n", n.x, n.y, n.z, n1.x, n1.y, n1.z, n2.x, n2.y, n2.z);
			
			double min_dis = fabs(vecd3::dot(n, p1-p2)/n.mag());
			vecd3 c1 = p1 + d1 * (vecd3::dot(p2 - p1, n2) / vecd3::dot(d1, n2));
			vecd3 c2 = p2 + d2 * (vecd3::dot(p1 - p2, n1) / vecd3::dot(d2, n1));
			vecd3 depth_v = c1 - p1;
			
//			printf("min_dis: %lf\n", min_dis);
//			printf("c1: (%.2lf, %.2lf, %.2lf), c2: (%.2lf, %.2lf, %.2lf)\n", c1.x, c1.y, c1.z, c2.x, c2.y, c2.z);
//			printf("depth_v: (%.2lf, %.2lf, %.2lf)\n", depth_v.x, depth_v.y, depth_v.z);
			
			vecd3 t = (c2 - p2) / d2;
			bool c2_is_on_seg = (t.x >= 0 && t.x <= 1) || (t.y >= 0 && t.y <= 1) || (t.z >= 0 && t.z <= 1);
			
//			printf("t:(%.2lf, %.2lf, %.2lf)\n", t.x, t.y, t.z);
			
//			printf("depth_v . d1: %.2f\n", vecd3::dot(d1, depth_v));
			
			if (min_dis <= 0.1 && vecd3::dot(d1, depth_v) > 0 && c2_is_on_seg) {
				return RGBAD(255, 255, 255, 255, depth_v.mag());
			}
		}
		
		return RGBAD(0, 0, 0, 0, INFINITY);
	}
	
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
		darray<vecd3> p_pts(m.p.size);
		for (int j = 0; j < m.p.size; j++) {
			if (m.p[j].x < c->clip_near) {
				continue;
			}
			
			p_pts[j].x =  (m.p[j].x - c->clip_near) / (c->clip_far - c->clip_near) * 2 - 1;
			p_pts[j].y =  m.p[j].y / (m.p[j].x * slope_x);
			p_pts[j].z = -m.p[j].z / (m.p[j].x * slope_y);
		}
		
		// Iterate over and render each edge.
		for (int j = 0; j < m.e.size; j++) {
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
				dif = dif / dif.x * (c->clip_near - A.x) + A;
				a.x =  (dif.x - c->clip_near) / (c->clip_far - c->clip_near) * 2 - 1;
				a.y =  dif.y / (dif.x * slope_x);
				a.z = -dif.z / (dif.x * slope_y);
			}
			else if (B.x < c->clip_near) {
				dif = A - B;
				dif = dif / dif.x * (c->clip_near - B.x) + B;
				b.x =  (dif.x - c->clip_near) / (c->clip_far - c->clip_near) * 2 - 1;
				b.y =  dif.y / (dif.x * slope_x);
				b.z = -dif.z / (dif.x * slope_y);
			}
			
			// If both points are too far off one side, skip them. Clipping off +/-x is handled above
			if (a.y > 1 && b.y > 1) {
				continue;
			}
			if (a.y < -1 && b.y < -1) {
				continue;
			}
			if (a.z > 1 && b.z > 1) {
				continue;
			}
			if (a.z < -1 && b.z < -1) {
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
				if (t > 0 && t < 1) {
					if (a.y > 1) {
						a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
					} else {
						b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
					}
					dif = b-a;
				}
				// Clip against -y
				t = 1 / dif.y * (-1 - a.y);
				if (t > 0 && t < 1) {
					if (a.y < -1) {
						a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
					} else {
						b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
					}
					dif = b-a;
				}
			}
			if (dif.z != 0) {
				// Clip against +z
				t = 1 / dif.z * (1 - a.z);
				if (t > 0 && t < 1) {
					if (a.z > 1) {
						a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
					} else {
						b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
					}
					dif = b-a;
				}
				// Clip against -z
				t = 1 / dif.z * (-1 - a.z);
				if (t > 0 && t < 1) {
					if (a.z < -1) {
						a = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
					} else {
						b = vecd3(a.x + t * dif.x, a.y + t * dif.y, a.z + t * dif.z);
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
	
	void raycast_render(app_handle* ah, cam* c, object* o, void* state, double dt) {}
}
