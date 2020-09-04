#include <stdio.h>

#include "sgl.hpp"

using namespace sgl;

class game_state {
public:
	cam c;
	object* o;
	int o_n;
	
	bool paused;
	
	// Pointer to the next space to occupy with a new bullet line. Points to indices 1-32 in the object array gs.o
	int bl_cur;
	
	// Timer since the game statrted
	double game_timer;
	
	// Current number of zombies in play. Maxes out at 20
	int zombie_num;
	
	// Health of the zombies
	int zombie_health[20];
	
	// Time since death of each zombie. Animated with the same code as bullet lines, except with a preceeding vertex split.
	double zombie_death_timer[20];
	
	// Timer used to control the gun firing animation and firing delay
	double shot_timer;
	
	// Timer used to control the bullet line animations
	double bl_timer[32];
	
	// Used for random number generator
	unsigned int seed[32];
	
	// Used to control character bobbing
	double step_pt;
	
	game_state();
};

game_state::game_state() : c(), o(NULL), o_n(0), paused(false), bl_cur(1), step_pt(0), game_timer(0), zombie_num(0), shot_timer(1) {}

void init(app_handle& ah, void* state) {
	ah.hide_pointer();
}

// WASD : 25 38 39 40
void draw(app_handle& ah, void* state, double dt) {
	game_state* gs = (game_state*) state;
	
	if (!gs->paused) {
		gs->game_timer += dt;
		
		// Number of zombies that should be in play at this time.
		int zombie_targ = fmin(gs->game_timer / 5, 20);
		
		// Spawn zombies
		if (gs->zombie_num < zombie_targ && rand() % 1000 / 1000 < dt) {
			// Find an open space in the zombie list
			int z_cur = 0;
			for (int i = 35; i < 155; i+=6) {
				if (gs->o[i].is_hidden) {
					z_cur = i;
					break;
				}
			}
			
			if (z_cur == 0) {
				printf("Error: attempted to spawn zombie but no space was available.\n");
			}
			else {
				vecd3 sp(rand() % 192 - 96, rand() % 192 - 96, 0);
				gs->o[z_cur  ].p = sp;
				gs->o[z_cur+1].p = sp;
				gs->o[z_cur+2].p = sp + vecd3(0, -0.7 , 0);
				gs->o[z_cur+3].p = sp + vecd3(0,  0.7 , 0);
				gs->o[z_cur+4].p = sp + vecd3(0, -0.25, 0);
				gs->o[z_cur+5].p = sp + vecd3(0,  0.25, 0);
				
				gs->zombie_health[(z_cur-35)/6] = 4;
				
				gs->zombie_num++;
				
				for (int i = z_cur; i < z_cur+6; i++) {
					gs->o[i].show();
				}
			}
		}
		
		// Used for bullet line animations
		vecd3 p1;
		vecd3 p2;
		vecd3 pivot;
		vecd3 off;
		quaternion rot;
		double modifier;
		
		// Make the bullet line duller and rotate the bullet line pieces
		for (int i = 0; i < 32; i++) {
			if (gs->o[i].color < 0xFFFFFF) {
				gs->o[i].color += 0x000404;
				
				gs->bl_timer[i] += dt;
				
				srand(gs->seed[i]);
				
				for (int j = 0; j < gs->o[i].m.en; j++) {
					modifier = pow(gs->bl_timer[i], 1.5) / 2;
					
					p1 = gs->o[i].m.p[gs->o[i].m.e[j].x];
					p2 = gs->o[i].m.p[gs->o[i].m.e[j].y];
					pivot = vecd3(rand() % 1000 / 1000.0, rand() % 1000 / 1000.0, rand() % 1000 / 1000.0) + (p1+p2)/2;
					
					off = vecd3((rand() % 200 - 100) / 10000.0, (rand() % 200 - 100) / 10000.0, (rand() % 200 - 100) / 10000.0);
					
					rot = quaternion(vecd3(rand() % 1000, rand() % 1000, rand() % 1000).normalize(), 0.004 * modifier);
					gs->o[i].m.p[gs->o[i].m.e[j].x] = quaternion::rotate(p1, pivot, rot) + off * modifier;
					gs->o[i].m.p[gs->o[i].m.e[j].y] = quaternion::rotate(p2, pivot, rot) + off * modifier;
				}
			}
		}
		
		// Teleport the mouse to the middle of the window
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
		double move_speed = 10 * dt;
		if (ah.get_key(50)) {
			move_speed *= 2;
			bob_speed = 40;
		}
		
		box_collider bx;
		bool space_free = true;
		vecd3 hit;
		
		// Handle movement and prevent out-of-bounds movement and movement into obstacles
		vecd3 new_pos;
		bool did_move = false;
		if (ah.get_key(25)) {
			did_move = true;
			vecd3 f = gs->c.forward();
			f = vecd3(f.x, f.y, 0).normalize(move_speed*1.5);
			for (int i = 155; i < gs->o_n; i++) {
				hit = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->c.p - vecd3(0, 0, 3.5), f);
				if (hit.is_nan() || (hit - (gs->c.p - vecd3(0, 0, 3.5))).mag() > move_speed*3) {
					continue;
				}
				space_free = false;
				break;
			}
			if (space_free) {
				new_pos = gs->c.p + f;
				if (new_pos.x < 96 && new_pos.x > -96 && new_pos.y < 96 && new_pos.y > -96) {
					gs->c.p = new_pos;
				}
			}
		}
		else if (ah.get_key(39)) {
			did_move = true;
			vecd3 f = gs->c.forward();
			f = -vecd3(f.x, f.y, 0).normalize(move_speed);
			for (int i = 155; i < gs->o_n; i++) {
				hit = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->c.p - vecd3(0, 0, 3.5), f);
				if (hit.is_nan() || (hit - (gs->c.p - vecd3(0, 0, 3.5))).mag() > move_speed*2) {
					continue;
				}
				space_free = false;
				break;
			}
			if (space_free) {
				new_pos = gs->c.p + f;
				if (new_pos.x < 96 && new_pos.x > -96 && new_pos.y < 96 && new_pos.y > -96) {
					gs->c.p = new_pos;
				}
			}
		}
		if (ah.get_key(38)) {
			did_move = true;
			vecd3 f = gs->c.forward();
			f = -vecd3(-f.y, f.x, 0).normalize(move_speed);
			for (int i = 155; i < gs->o_n; i++) {
				hit = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->c.p - vecd3(0, 0, 3.5), f);
				if (hit.is_nan() || (hit - (gs->c.p - vecd3(0, 0, 3.5))).mag() > move_speed*2) {
					continue;
				}
				space_free = false;
				break;
			}
			if (space_free) {
				new_pos = gs->c.p + f;
				if (new_pos.x < 96 && new_pos.x > -96 && new_pos.y < 96 && new_pos.y > -96) {
					gs->c.p = new_pos;
				}
			}
		}
		else if (ah.get_key(40)) {
			did_move = true;
			vecd3 f = gs->c.forward();
			f =  vecd3(-f.y, f.x, 0).normalize(move_speed);
			for (int i = 155; i < gs->o_n; i++) {
				hit = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->c.p - vecd3(0, 0, 3.5), f);
				if (hit.is_nan() || (hit - (gs->c.p - vecd3(0, 0, 3.5))).mag() > move_speed*2) {
					continue;
				}
				space_free = false;
				break;
			}
			if (space_free) {
				new_pos = gs->c.p + f;
				if (new_pos.x < 96 && new_pos.x > -96 && new_pos.y < 96 && new_pos.y > -96) {
					gs->c.p = new_pos;
				}
			}
		}
	
		if (did_move) {
			// Character bobbing
			gs->step_pt += bob_speed * dt;
			gs->c.translate(vecd3(0, 0, sin(gs->step_pt) / 40));
			if (gs->step_pt > 6.283) {
				gs->step_pt = 0;
				gs->c.p = vecd3(gs->c.p.x, gs->c.p.y, 4);
			}
		}
	
		// Move objects to character
		gs->o[33].r = gs->c.r;
		gs->o[33].p = gs->c.p + gs->c.forward()*1.3 - gs->c.up()*0.4 + gs->c.right()*0.5;
		gs->o[34].r = gs->c.r;
		gs->o[34].p = gs->c.p + gs->c.forward()*1.3 - gs->c.up()*0.4 + gs->c.right()*0.5;
		
		gs->shot_timer += dt * 8;
		// Increase shot_timer
		if (gs->shot_timer < 1) {
			// Rotate the gun based on the shot timer
			gs->o[33].rotate(quaternion(gs->c.right(), -sin(gs->shot_timer * 3.14159) * 0.2));
			
			gs->o[34].rotate(quaternion(gs->c.right(), -sin(gs->shot_timer * 3.14159) * 0.2));
			gs->o[34].translate(vecd3(-1, 0, 0) * sin(gs->shot_timer * 3.14159) * 0.3, false);
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
	game_state* gs = (game_state*) state;
	
	if (e.code == 1 && gs->shot_timer >= 1 && !gs->paused) {
		// Reset the shot timer
		gs->shot_timer = 0;
		gs->bl_timer[gs->bl_cur] = 0;
		
		// Color the bullet line
		gs->o[gs->bl_cur].color = 0xFF0303;
		int bl_cur = gs->bl_cur;
		gs->bl_cur = gs->bl_cur + 1;
		if (gs->bl_cur == 32) {
			gs->bl_cur = 0;
		}
		
		vecd3 floor_hit;
		vecd3 hit;
		vecd3 hit_temp;
		box_collider bx;
		
		// Index of object hit, discrens between body hits and head hits for zombies.
		int obj_hit = -1;
		
		// Impact with environment
		hit = bx.raycast(gs->o[155].p, gs->o[155].r, gs->o[155].s, gs->o[34].p, gs->o[34].forward());
		if (!hit.is_nan()) {obj_hit = 155;}
		for (int i = 156; i < gs->o_n; i++) {
			hit_temp = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->o[34].p, gs->o[34].forward());
			if (!hit_temp.is_nan()) {
				if ((hit - gs->o[34].p).sqr_mag() > (hit_temp - gs->o[34].p).sqr_mag() || hit.is_nan()) {
					hit = hit_temp;
					obj_hit = i;
				}
			}
		}
		
		// Collide with floor
		bx.zp = 0;
		floor_hit = bx.raycast(gs->o[32].p, gs->o[32].r, gs->o[32].s, gs->o[34].p, gs->o[34].forward());
		if (!floor_hit.is_nan()) {
			if ((hit - gs->o[34].p).sqr_mag() > (floor_hit - gs->o[34].p).sqr_mag() || hit.is_nan()) {
				hit = floor_hit;
				obj_hit = 32;
			}
		}
		
		// Adjust box collider for zombie bodies
		bx.zn =  0;
		bx.zp =  3.6;
		bx.yn = -0.9;
		bx.yp =  0.9;
		bx.xn = -0.25;
		bx.xp =  0.25;
		
		for (int i = 36; i < 155; i+=6) {
			hit_temp = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->o[34].p, gs->o[34].forward());
			if (!hit_temp.is_nan()) {
				if ((hit - gs->o[34].p).sqr_mag() > (hit_temp - gs->o[34].p).sqr_mag() || hit.is_nan()) {
					hit = hit_temp;
					obj_hit = i;
				}
			}
		}
		
		// Adjust box collider for zombie heads
		bx.zn =  3.6;
		bx.zp =  4.4;
		bx.yn = -0.4;
		bx.yp =  0.4;
		bx.xn = -0.4;
		bx.xp =  0.4;
		
		for (int i = 35; i < 155; i+=6) {
			hit_temp = bx.raycast(gs->o[i].p, gs->o[i].r, gs->o[i].s, gs->o[34].p, gs->o[34].forward());
			if (!hit_temp.is_nan()) {
				if ((hit - gs->o[34].p).sqr_mag() > (hit_temp - gs->o[34].p).sqr_mag() || hit.is_nan()) {
					hit = hit_temp;
					obj_hit = i;
				}
			}
		}
		
		if (hit.is_nan()) {
			hit = gs->o[34].forward() * 96 + gs->o[34].p;
		}
		
		int zom_hit;
		
		if (obj_hit == -1) {
//			printf("Hit nothing\n");
		} else if (obj_hit >= 155) {
//			printf("Hit environment\n");
		} else if (obj_hit == 32) {
//			printf("Hit floor\n");
		} 
		// Headshot
		else if ((obj_hit - 35) % 6 == 0) {
			zom_hit = (obj_hit-35)/6;
			gs->zombie_health[zom_hit] -= 2;
		}
		// Body shot
		else if ((obj_hit - 36) % 6 == 0) {
			zom_hit = (obj_hit-36)/6;
			gs->zombie_health[zom_hit] -= 1;
		}
		
		printf("%d\n", zombie_health);
		if (gs->zombie_health[zom_hit] <= 0) {
			for (int i = 0; i < 6; i++) {
				gs->o[zom_hit*6+35 + i].hide();
			}
		}
		
		gs->seed[bl_cur] = rand();
		
		// Create bullet line
		vecd3 hit_vec = hit - gs->o[34].p;
		double hit_len = hit_vec.mag();
		double line_len = 2;
		int num_lines = ceil(hit_len / line_len);
		gs->o[bl_cur].m.pn = num_lines * 2;
		gs->o[bl_cur].m.en = num_lines;
		gs->o[bl_cur].m.p = darray<vecd3>(num_lines * 2);
		gs->o[bl_cur].m.e = darray<veci2>(num_lines);
		
		vecd3 end;
		gs->o[bl_cur].m.p[0] = gs->o[34].p;
		for (int i = 0; i < num_lines-1; i++) {
			end = gs->o[34].p + hit_vec * ((double) (i+1)/num_lines);
			gs->o[bl_cur].m.p[i*2+1] = end;
			gs->o[bl_cur].m.p[i*2+2] = end;
			gs->o[bl_cur].m.e[i] = veci2(i*2, i*2+1);
		}
		gs->o[bl_cur].m.p[num_lines*2-1] = hit;
		gs->o[bl_cur].m.e[num_lines-1] = veci2(num_lines*2-2, num_lines*2-1);
	}
}

// o_n = 34
// o[0]   - o[31]  = bullet lines
// o[32]           = floor
// o[33]           = pistol
// o[34]           = pistol_slide
// o[35]  - o[154] = zombies
// o[155] - o[170] = environment
int main() {
	game_state gs;
	
	// seed RNG
	srand(0);
	for (int i = 0; i < 32; i++) {
		gs.seed[i] = rand();
	}
	
	// Position camera
	gs.c.translate(vecd3(0, 0, 4));
	
	// Create space for objects
	gs.o_n = 170;
	gs.o = new object[gs.o_n];
	
	for (int i = 0; i < 32; i++) {
		gs.o[i].color = 0xFFFFFF;
		gs.bl_timer[i] = 0;
	}
	
	// Create floor
	gs.o[32].m = mesh_wire(mesh_wire::GRID, 33, 33);
	gs.o[32].scale(vecd3(96, 96, 0));
	
	// Create Pistol
	mesh_wire pistol((char*) "./models/pistol.v3d");
	gs.o[33].m = pistol;
	gs.o[33].scale(vecd3(0.1, 0.1, 0.1));
	
	mesh_wire pistol_slide((char*) "./models/pistol_slide.v3d");
	gs.o[34].m = pistol_slide;
	gs.o[34].scale(vecd3(0.1, 0.1, 0.1));
	
	// Create environment
	gs.o[155].m = mesh_wire(mesh_wire::CUBE);
	gs.o[155].translate(vecd3(15, 21, 3));
	gs.o[155].rotate(vecd3(0, 0, 1), -3.14159/4);
	gs.o[155].scale(vecd3(4, 0.5, 3));
	
	gs.o[156].m = mesh_wire(mesh_wire::CUBE);
	gs.o[156].translate(vecd3(-26, 19, 1));
	gs.o[156].scale(vecd3(1, 2, 1));
	
	gs.o[157].m = mesh_wire(mesh_wire::CUBE);
	gs.o[157].translate(vecd3(-25, 25, 1));
	gs.o[157].rotate(vecd3(0, 0, 1), -3.14159/8);
	gs.o[157].scale(vecd3(1, 2, 1));
	
	gs.o[158].m = mesh_wire(mesh_wire::CUBE);
	gs.o[158].translate(vecd3(-21, 30, 1));
	gs.o[158].rotate(vecd3(0, 0, 1), -3.14159/4);
	gs.o[158].scale(vecd3(1, 2, 1));
	
	gs.o[159].m = mesh_wire(mesh_wire::CUBE);
	gs.o[159].translate(vecd3(47, 3, 4));
	gs.o[159].scale(vecd3(4, 4, 4));
	
	gs.o[160].m = mesh_wire(mesh_wire::CUBE);
	gs.o[160].translate(vecd3(9, -45, 1));
	gs.o[160].scale(vecd3(8, 0.5, 1));
	
	gs.o[161].m = mesh_wire(mesh_wire::CUBE);
	gs.o[161].translate(vecd3(69, 48, 1));
	gs.o[161].rotate(vecd3(0, 0, 1), -3.14159/4);
	gs.o[161].scale(vecd3(8, 0.5, 1));
	
	gs.o[162].m = mesh_wire(mesh_wire::CUBE);
	gs.o[162].translate(vecd3(-55, -55, 8));
	gs.o[162].scale(vecd3(2, 2, 8));
	
	gs.o[163].m = mesh_wire(mesh_wire::CUBE);
	gs.o[163].translate(vecd3(-2, -69, 3));
	gs.o[163].scale(vecd3(1, 5, 3));
	
	gs.o[164].m = mesh_wire(mesh_wire::CUBE);
	gs.o[164].translate(vecd3(55, -62, 1));
	gs.o[164].scale(vecd3(2, 1, 1));
	
	gs.o[165].m = mesh_wire(mesh_wire::CUBE);
	gs.o[165].translate(vecd3(39, 52, 1));
	
	gs.o[166].m = mesh_wire(mesh_wire::CUBE);
	gs.o[166].translate(vecd3(-72, -17, 3));
	gs.o[166].rotate(vecd3(0, 0, 1), 3.14159/8);
	gs.o[166].scale(vecd3(1, 2, 3));
	
	gs.o[167].m = mesh_wire(mesh_wire::CUBE);
	gs.o[167].translate(vecd3(-20, -7, 3));
	gs.o[167].scale(vecd3(1, 2, 3));
	
	gs.o[168].m = mesh_wire(mesh_wire::CUBE);
	gs.o[168].translate(vecd3(5, -9, 2));
	gs.o[168].scale(vecd3(5, 2, 2));
	
	gs.o[169].m = mesh_wire(mesh_wire::CUBE);
	gs.o[169].translate(vecd3(-64, 52, 4));
	gs.o[169].scale(vecd3(4, 4, 4));
	
	// Make zombies
	for (int i = 35; i < 155; i+=6) {
		gs.o[i  ].m = mesh_wire((char*) "./models/z_head.v3d");
		
		gs.o[i+1].m = mesh_wire((char*) "./models/z_body.v3d");
		
		gs.o[i+2].m = mesh_wire((char*) "./models/z_arm.v3d");
		gs.o[i+2].translate(vecd3(0, -0.7, 0));
		
		gs.o[i+3].m = mesh_wire((char*) "./models/z_arm.v3d");
		gs.o[i+3].translate(vecd3(0, 0.7,0));
		
		gs.o[i+4].m = mesh_wire((char*) "./models/z_leg.v3d");
		gs.o[i+4].translate(vecd3(0, -0.25, 0));
		
		gs.o[i+5].m = mesh_wire((char*) "./models/z_leg.v3d");
		gs.o[i+5].translate(vecd3(0, 0.25, 0));
	}
	
	for (int i = 0; i < 20; i++) {
		gs.zombie_health[i] = 0;
		gs.zombie_death_timer[i] = 0;
	}
	
	// Color & hide Zombies
	for (int i = 35; i < 155; i++) {
		gs.o[i].color = 0x00AA00;
		gs.o[i].hide();
	}
	
	// Create bullet lines
	for (int i = 0; i < 32; i++) {
		gs.o[i].m.p = darray<vecd3>(2);
		gs.o[i].m.p[0] = vecd3(0, 0, 0);
		gs.o[i].m.p[1] = vecd3(0, 0, 0);
		
		gs.o[i].m.e = darray<veci2>(1);
		gs.o[i].m.e[0] = veci2(0, 1);
		gs.o[i].m.pn = 2;
		gs.o[i].m.en = 1;
		
		gs.o[i].color = 0xFF0000;
	}
	
	// Create window and start mainloop
	init_data id = {800, 800, 1, (char*) "Voxel", 0xFFFFFF, 0x0};
	
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









