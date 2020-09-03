#include <stdio.h>

#include "sgl.hpp"

using namespace sgl;

class game_state {
public:
	cam c;
	object* o;
	int o_n;
	
	bool paused;
	
	bool ball_grabbing;
	
	// Used to control character bobbing
	double step_pt;
	
	game_state();
};

game_state::game_state() : c(), o(NULL), o_n(0), step_pt(0), paused(false), ball_grabbing(false) {}

void init(app_handle& ah, void* state) {
	ah.hide_pointer();
}

// WASD : 25 38 39 40
void draw(app_handle& ah, void* state, double dt) {
	game_state* gs = (game_state*) state;
	
	if (!gs->paused) {
		if (ah.get_hover()) {
			veci2 cp = ah.get_pointer();
			if (cp.x != ah.win_w/2 || cp.y != ah.win_h/2) {
				double theta_x = (cp.x - ah.win_w/2) / 200.0;
				double theta_y = (cp.y - ah.win_h/2) / 200.0;
	
				gs->c.rotate(sgl::vecd3(0, 0, 1), theta_x);
				gs->c.rotate(sgl::vecd3(0, 1, 0), theta_y, false);
		
				ah.set_pointer(ah.win_w/2, ah.win_h/2);
			}
		}
	
		double bob_speed = 20;
		double move_speed = 0.2;
		if (ah.get_key(50)) {
			move_speed = 0.4;
			bob_speed = 40;
		}
		
		box_collider bx;
		bool space_free = true;
		vecd3 hit;
	
		bool did_move = false;
	
		if (ah.get_key(25)) {
			did_move = true;
			vecd3 f = gs->c.facing();
			f = vecd3(f.x, f.y, 0).normalize(move_speed*1.5);
			for (int i = 2; i < gs->o_n; i++) {
				hit = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->c.p - vecd3(0, 0, 3.5), f);
				if (hit.is_nan() || (hit - (gs->c.p - vecd3(0, 0, 3.5))).mag() > move_speed*3) {
					continue;
				}
				space_free = false;
				break;
			}
			if (space_free) {
				gs->c.translate(f);
			}
		}
		else if (ah.get_key(39)) {
			did_move = true;
			vecd3 f = gs->c.facing();
			f = -vecd3(f.x, f.y, 0).normalize(move_speed);
			for (int i = 2; i < gs->o_n; i++) {
				hit = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->c.p - vecd3(0, 0, 3.5), f);
				if (hit.is_nan() || (hit - (gs->c.p - vecd3(0, 0, 3.5))).mag() > move_speed*2) {
					continue;
				}
				space_free = false;
				break;
			}
			if (space_free) {
				gs->c.translate(f);
			}
		}
		if (ah.get_key(38)) {
			did_move = true;
			vecd3 f = gs->c.facing();
			f = -vecd3(-f.y, f.x, 0).normalize(move_speed);
			for (int i = 2; i < gs->o_n; i++) {
				hit = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->c.p - vecd3(0, 0, 3.5), f);
				if (hit.is_nan() || (hit - (gs->c.p - vecd3(0, 0, 3.5))).mag() > move_speed*2) {
					continue;
				}
				space_free = false;
				break;
			}
			if (space_free) {
				gs->c.translate(f);
			}
		}
		else if (ah.get_key(40)) {
			did_move = true;
			vecd3 f = gs->c.facing();
			f =  vecd3(-f.y, f.x, 0).normalize(move_speed);
			for (int i = 2; i < gs->o_n; i++) {
				hit = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->c.p - vecd3(0, 0, 3.5), f);
				if (hit.is_nan() || (hit - (gs->c.p - vecd3(0, 0, 3.5))).mag() > move_speed*2) {
					continue;
				}
				space_free = false;
				break;
			}
			if (space_free) {
				gs->c.translate(f);
			}
		}
	
		if (did_move) {
			gs->step_pt += bob_speed * dt;
			gs->c.translate(vecd3(0, 0, sin(gs->step_pt) / 40));
			if (gs->step_pt > 6.283) {
				gs->step_pt = 0;
				gs->c.p = vecd3(gs->c.p.x, gs->c.p.y, 4);
			}
		}
	}
	
	ah.clear_display();
	ah.render(gs->c, gs->o, gs->o_n);
	ah.update_display();
}

void key_press(event e, app_handle& ah, void* state) {
	game_state* gs = (game_state*) state;
	
	//printf("%d\n", e.code);
	if (e.code == 9) {
		gs->paused = !gs->paused;
		
		if (gs->paused) {
			ah.show_pointer();
		} else {
			ah.hide_pointer();
			ah.set_pointer(ah.win_w/2, ah.win_h/2);
		}
	}
}

void click(event e, app_handle& ah, void* state) {
	if (e.code == 1) {
		game_state* gs = (game_state*) state;
		
		vecd3 hit;
		box_collider bx;
		hit = bx.raycast(gs->o[2].p, gs->o[2].r, gs->o[2].s, gs->c.p, gs->c.facing());
		
		gs->o[1].m.p[0] = gs->c.p;
		gs->o[1].m.p[1] = gs->c.facing() * 50 + gs->c.p;
		if (!hit.is_nan()) {
			gs->o[1].m.p[1] = hit;
		}
	}
}

int main() {
	game_state gs;
	gs.o_n = 3;
	gs.o = new object[3];
	
	gs.o[0].m = mesh_wire(mesh_wire::GRID, 33, 33);
	gs.o[2].m = mesh_wire(mesh_wire::CUBE, 12, 12);
	
	gs.c.translate(vecd3(0, 0, 4));
	gs.o[0].scale(vecd3(99, 99, 0));
	init_data id = {800, 800, 1, (char*) "Voxel", 0xFFFFFF, 0x0};
	
//	gs.o[1].translate(vecd3(0, 0, 1));
	
	gs.o[2].translate(vecd3(2, 2, 3));
	gs.o[2].rotate(vecd3(0, 0, 1), 3.14159/4);
	gs.o[2].scale(vecd3(2, 1, 3));
	
	darray<vecd3> p(2);
	p[0] = vecd3(0, 0, 0);
	p[1] = vecd3(0, 0, 0);
	darray<veci2> e(1);
	e[0] = veci2(0, 1);
	gs.o[1].m.pn = 2;
	gs.o[1].m.en = 1;
	gs.o[1].m.p  = p;
	gs.o[1].m.e  = e;
	
	event_map em;
	em.init = init;
	em.draw = draw;
	em.key_press = key_press;
	em.button_press = click;
	em.state = (void*) &gs;
	
	app_handle ah;
	ah.mainloop(id, em);
	
	delete[] gs.o;
	
	return 0;
}









