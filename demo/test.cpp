#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "sgl.hpp"

class cube_cursor {
public:
	int cx;
	int cy;
	
	int opt1;
	int opt2;
	int p;
	
	sgl::cam c;
	
	sgl::object o;
	
	cube_cursor();
};

cube_cursor::cube_cursor() : cx(0), cy(0), opt1(64), opt2(32), p(sgl::mesh_wire::CUBE), c() {}

void draw(sgl::app_handle& ah, void* state, double dt) {
	cube_cursor* cc = (cube_cursor*) state;
	
	if (ah.get_key(25)) {
		cc->c.translate(sgl::vecd3( 0.03, 0, 0), false);
	}
	if (ah.get_key(39)) {
		cc->c.translate(sgl::vecd3(-0.03, 0, 0), false);
	}
	if (ah.get_key(38)) {
		cc->c.translate(sgl::vecd3(0, -0.03, 0), false);
	}
	if (ah.get_key(40)) {
		cc->c.translate(sgl::vecd3(0,  0.03, 0), false);
	}
	if (ah.get_key(24)) {
		cc->c.translate(sgl::vecd3(0, 0, -0.03), false);
	}
	if (ah.get_key(26)) {
		cc->c.translate(sgl::vecd3(0, 0,  0.03), false);
	}
	
	ah.clear_display();
	ah.render(cc->c, &cc->o, 1);
	ah.update_display();
}

void on_button(sgl::event e, sgl::app_handle& ah, void* state) {
	if (e.code == 1) {
		cube_cursor* cc = (cube_cursor*) state;
		
		cc->cx = e.cx;
		cc->cy = e.cy;
	}
}

// WASD : 25 38 39 40
void on_key(sgl::event e, sgl::app_handle& ah, void* state) {
	cube_cursor* cc = (cube_cursor*) state;
//	printf("%d\n", e.code);
	
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
	} else {
		return;
	}
	if (cc->p == sgl::mesh_wire::ICOSAHEDRON) {
		int temp = cc->opt2 - 2;
		if (temp > 3) {
			temp = 3;
		}
		sgl::mesh_wire m((sgl::mesh_wire::primtype) cc->p, temp, cc->opt2, 0.5);
		cc->o.m = m;
	} else if (cc->p == sgl::mesh_wire::CYLINDER) {
		sgl::mesh_wire m((sgl::mesh_wire::primtype) cc->p, cc->opt2);
		cc->o.m = m;
	} else if (cc->p == sgl::mesh_wire::GRID) {
		sgl::mesh_wire m((sgl::mesh_wire::primtype) cc->p, cc->opt1/2, cc->opt2);
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
		
		cc->c.rotate(sgl::vecd3(0, 0, 1), theta_x);
		cc->c.rotate(sgl::vecd3(0, 1, 0), theta_y, false);
	}
}

int main() {
	cube_cursor cc;
	sgl::mesh_wire m(sgl::mesh_wire::CUBE);
	cc.o.m = m;
	cc.c.translate(sgl::vecd3(-4, 0, 0));
	cc.c.clip_near = 0.01;
	
	sgl::init_data id = {800, 800, 1, (char*) "Voxel", 0xFFFFFF, 0x0};
	sgl::event_map em;
	
	em.draw = draw;
	em.button_press = on_button;
	em.key_press = on_key;
	em.key_repeat = on_key;
	em.motion = drag;
	em.state = (void*) &cc;
	
	sgl::app_handle ah;
	ah.mainloop(id, em);
	
	return 0;
}
