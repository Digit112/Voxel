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
	
	sgl::object o;
	
	cube_cursor();
};

cube_cursor::cube_cursor() : cx(0), cy(0), m(sgl::mesh_wire()) {}

void draw(sgl::app_handle& ah, void* state) {
	cube_cursor* cc = (cube_cursor*) state;
	
	double theta = 60 * (3.14159/180);
	
	sgl::veci2 sp[8];
	for (int i = 0; i < cc->m.pn; i++) {
		sp[i].x = (int) ((cc->m.p[i].y / (cc->m.p[i].x * tan(theta/2)) + 1) / 2 * ah.win_h);
		sp[i].y = (int) ((cc->m.p[i].z / (cc->m.p[i].x * tan(theta/2)) + 1) / 2 * ah.win_h);
	}
	
	ah.clear_display();
	for (int i = 0; i < cc->m.en; i++) {
		ah.draw_line(sp[cc->m.e[i].x].x, sp[cc->m.e[i].x].y, sp[cc->m.e[i].y].x, sp[cc->m.e[i].y].y);
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
		

void drag(sgl::event e, sgl::app_handle& ah, void* state) {
	if (ah.get_button(1)) {
		cube_cursor* cc = (cube_cursor*) state;
		
		double theta_x = (e.cx - cc->cx) / 200.0;
		double theta_y = (e.cy - cc->cy) / 200.0;
		
		cc->cx = e.cx;
		cc->cy = e.cy;
		
		for (int i = 0; i < cc->m.pn; i++) {
			cc->m.p[i] = sgl::quaternion::rotate(cc->m.p[i], sgl::vecd3(4, 0, 0), sgl::vecd3(0, 0, 1), -theta_x);
			cc->m.p[i] = sgl::quaternion::rotate(cc->m.p[i], sgl::vecd3(4, 0, 0), sgl::vecd3(0, 1, 0), theta_y);
		}
		//draw(ah, state);
	}
}

int main() {	
	cube_cursor cc;
	sgl::mesh_wire m(sgl::mesh_wire::CUBE);
	cc.m = m;
	
	sgl::init_data id = {800, 800, 1, (char*) "Voxel", 0xFFFFFF, 0x0};
	sgl::event_map em;
	
	em.draw = draw;
	em.button_press = press;
	em.motion = drag;
	em.state = (void*) &cc;
	
	sgl::app_handle ah;
	ah.mainloop(id, em);
	
	return 0;
}
