#include <stdio.h>

#include "sgl.hpp"

using namespace sgl;

class game_state {
public:
	cam c;
	object* o;
	int o_n;
	
	game_state();
};

game_state::game_state() : c(), o(NULL) {}

void draw(app_handle& ah, void* state) {
	game_state* gs = (game_state*) state;
	
	ah.clear_display();
	ah.render(gs->c, gs->o, gs->o_n);
	ah.update_display();
}

int main() {
	game_state gs;
	gs.o_n = 2;
	gs.o = new object[2];
	
	gs.o[0].m = mesh_wire(mesh_wire::GRID, 33, 33);
	gs.o[1].m = mesh_wire(mesh_wire::CUBE);
	
	gs.c.translate(vecd3(0, 0, -2));
	
	init_data id = {800, 800, 1, (char*) "Voxel", 0xFFFFFF, 0x0};
	
	event_map em;
	em.draw = draw;
	em.state = (void*) &gs;
	
	app_handle ah;
	ah.mainloop(id, em);
	
	delete[] gs.o;
	
	return 0;
}









