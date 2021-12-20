#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "sgl.hpp"

using namespace sgl;

class cube_cursor {
public:
	int cx;
	int cy;
	
	int opt1;
	int opt2;
	int p;
	
	render_group rg;
	
	cam c;
	
	object o;
	
	cube_cursor();
};

expr<RGBAD> rend(app_handle* ah, vecd3 pos, vecd3 dir, void* state, double delta_time,
		             darray<expr<RGBAD>>& render_results, darray<expr<RGBAD>>& object_results) {
	return object_results[0];
}

cube_cursor::cube_cursor() : cx(0), cy(0), opt1(64), opt2(32), p(wire_mesh::CUBE), rg(1, 0), c() {}

void draw(app_handle& ah, void* state, double dt) {
	printf("%.2f\n", dt);
	cube_cursor* cc = (cube_cursor*) state;
	
	if (ah.get_key(25)) {
		cc->c.translate(vecd3( 0.03, 0, 0), false);
	}
	if (ah.get_key(39)) {
		cc->c.translate(vecd3(-0.03, 0, 0), false);
	}
	if (ah.get_key(38)) {
		cc->c.translate(vecd3(0, -0.03, 0), false);
	}
	if (ah.get_key(40)) {
		cc->c.translate(vecd3(0,  0.03, 0), false);
	}
	if (ah.get_key(24)) {
		cc->c.translate(vecd3(0, 0, -0.03), false);
	}
	if (ah.get_key(26)) {
		cc->c.translate(vecd3(0, 0,  0.03), false);
	}
	
	ah.clear_display();
	ah.render_rg(cc->c, cc->rg, state, dt);
	ah.update_display();
}

void on_button(event e, app_handle& ah, void* state) {
	if (e.code == 1) {
		cube_cursor* cc = (cube_cursor*) state;
		
		cc->cx = e.cx;
		cc->cy = e.cy;
	}
}

// WASD : 25 38 39 40
void on_key(event e, app_handle& ah, void* state) {
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
		if (cc->p > wire_mesh::CUBE) {
			cc->p = cc->p - 1;
		}
	}
	else if (e.code == 114) {
		if (cc->p < wire_mesh::TORUS) {
			cc->p = cc->p + 1;
		}
	} else {
		return;
	}
	if (cc->p == wire_mesh::ICOSAHEDRON) {
		int temp = cc->opt2 - 2;
		if (temp > 3) {
			temp = 3;
		}
		wire_mesh m((wire_mesh::primtype) cc->p, temp, cc->opt2, 0.5);
		cc->o.m = m;
	} else if (cc->p == wire_mesh::CYLINDER) {
		wire_mesh m((wire_mesh::primtype) cc->p, cc->opt2);
		cc->o.m = m;
	} else if (cc->p == wire_mesh::GRID) {
		wire_mesh m((wire_mesh::primtype) cc->p, cc->opt1/2, cc->opt2);
		cc->o.m = m;
	} else {
		wire_mesh m((wire_mesh::primtype) cc->p, cc->opt1, cc->opt2, 0.5);
		cc->o.m = m;
	}
}

void drag(event e, app_handle& ah, void* state) {
	if (ah.get_button(1)) {
		cube_cursor* cc = (cube_cursor*) state;
		
		double theta_x = (e.cx - cc->cx) / 200.0;
		double theta_y = (e.cy - cc->cy) / 200.0;
		
		cc->cx = e.cx;
		cc->cy = e.cy;
		
		cc->c.rotate(vecd3(0, 0, 1), theta_x);
		cc->c.rotate(vecd3(0, 1, 0), theta_y, false);
	}
}

int X_error_handler(Display* d, XErrorEvent* e) {
	char buff[256];
	XGetErrorText(d, e->error_code, buff, 256);
	printf("%s\n", buff);
	return 0;
}

int main() {
	XSetErrorHandler(X_error_handler);
	
	cube_cursor cc;
	cc.rg.render = rend;
	cc.rg.objects[0] = &cc.o;
	
	cc.o.m = wire_mesh(wire_mesh::CUBE);
	cc.o.render = wire_mesh_renderer;
	cc.c.translate(vecd3(-4, 0, 0));
	cc.c.clip_near = 0.01;
	
	init_data id = {100, 100, 1, (char*) "Voxel", 0xFFFFFF, 0x0};
	event_map em;
	
	em.draw = draw;
	em.button_press = on_button;
	em.key_press = on_key;
	em.key_repeat = on_key;
	em.motion = drag;
	em.state = (void*) &cc;
	
	app_handle ah;
	
	printf("%d\n", wire_mesh_renderer(&ah, &cc.o, cc.c.p, vecd3(3, 1.05, 0), NULL, 0).evaluate().R);
	
	ah.mainloop(id, em);
	
	return 0;
}
