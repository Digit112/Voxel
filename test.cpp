#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "sgl.hpp"
#include "sgl_mesh.hpp"
#include "sgl_vec.hpp"
#include "sgl_object.hpp"

class cube_cursor {
public:
	int cx;
	int cy;
	
	int theta;
	
	int opt1;
	int opt2;
	int p;
	
	sgl::object o;
	
	cube_cursor();
};

cube_cursor::cube_cursor() : cx(0), cy(0), theta(60), opt1(64), opt2(32), p(sgl::mesh_wire::CUBE) {}

void draw(sgl::app_handle& ah, void* state) {
	cube_cursor* cc = (cube_cursor*) state;
	
	srand(0);
	
	sgl::mesh_wire m;
	cc->o.applied(m);
	
	if (ah.get_key(38)) {
		ah.clear_display();
		cc->o.r = sgl::quaternion(1, 0, 0, 0);
	}
	
	double theta = cc->theta * (3.14159/180);
	
	ah.clear_display();
	
	sgl::veci2 sp[m.pn];
	for (int i = 0; i < m.pn; i++) {
		sp[i].x = (int) ((m.p[i].y / (m.p[i].x * tan(theta/2)) + 1) / 2 * ah.win_h);
		sp[i].y = (int) ((m.p[i].z / (m.p[i].x * tan(theta/2)) + 1) / 2 * ah.win_h);
		
//		ah.draw_rect(sp[i].x-1, sp[i].y-1, sp[i].x+1, sp[i].y+1);
	}
//	ah.clear_display();
	
	for (int i = 0; i < m.en; i++) {
		ah.draw_line(sp[m.e[i].x].x, sp[m.e[i].x].y, sp[m.e[i].y].x, sp[m.e[i].y].y);
	}
	ah.update_display();
}

void press(sgl::event e, sgl::app_handle& ah, void* state) {
	if (e.code == 1) {
		cube_cursor* cc = (cube_cursor*) state;
		
		cc->cx = e.cx;
		cc->cy = e.cy;
	}
}

void on_button(sgl::event e, sgl::app_handle& ah, void* state) {
	cube_cursor* cc = (cube_cursor*) state;
	
	if (e.code == 111) {
		cc->opt1 += 2;
		cc->opt2 ++;
	}
	else if (e.code == 116) {
		if (cc->opt2 > 3) {
			cc->opt1 -= 2;
			cc->opt2 --;
		}
	}
	else if (e.code == 113) {
		if (cc->p > sgl::mesh_wire::CUBE) {
			cc->p = cc->p - 1;
		}
	}
	else if (e.code == 114) {
		if (cc->p < sgl::mesh_wire::TORUS) {
			cc->p = cc->p + 1;
		}
	}
	if (cc->p == sgl::mesh_wire::ICOSAHEDRON) {
		int temp = cc->opt2 - 2;
		if (temp > 3) {
			temp = 3;
		}
		sgl::mesh_wire m((sgl::mesh_wire::primtype) cc->p, temp, cc->opt2, 0.5);
		cc->o.m = m;
	} else {
		sgl::mesh_wire m((sgl::mesh_wire::primtype) cc->p, cc->opt1, cc->opt2, 0.5);
		cc->o.m = m;
	}
}

void drag(sgl::event e, sgl::app_handle& ah, void* state) {
	if (ah.get_button(1)) {
		cube_cursor* cc = (cube_cursor*) state;
		
		double theta_x = (e.cx - cc->cx) / 200.0;
		double theta_y = (e.cy - cc->cy) / 200.0;
		
		cc->cx = e.cx;
		cc->cy = e.cy;
		
		cc->o.rotate(sgl::vecd3(0, 0, 1), -theta_x);
		cc->o.rotate(sgl::vecd3(0, 1, 0),  theta_y);
	}
}

int main() {
	cube_cursor cc;
	sgl::mesh_wire m(sgl::mesh_wire::CUBE);
	cc.o.m = m;
	cc.o.translate(sgl::vecd3(4, 0, 0));
	
	sgl::init_data id = {800, 800, 1, (char*) "Voxel", 0xFFFFFF, 0x0};
	sgl::event_map em;
	
	em.draw = draw;
	em.button_press = press;
	em.key_press = on_button;
	em.key_repeat = on_button;
	em.motion = drag;
	em.state = (void*) &cc;
	
	sgl::app_handle ah;
	ah.mainloop(id, em);
	
	return 0;
}
