#include <stdio.h>
#include "sgl.hpp"

using namespace sgl;

void draw(app_handle& ah, void* state, double dt) {
	object* root = (object*) state;
	cam c;
	
	root->scale(vecd3(1, 0.999, 1));
	root->rotate(vecd3(0, 0, 1), 0.02);
	
	ah.clear_display();
	ah.render(c, *root);
	ah.update_display();
}

int main() {
	object root(2);
	root.m = wire_mesh(wire_mesh::CUBE);
	root.translate(vecd3(0, 0, 2));
	
	object r_a(1);
	r_a.m = wire_mesh(wire_mesh::UVSPHERE, 12, 12);
	r_a.translate(vecd3(0, 2, 0));
	
	object r_b;
	r_b.m = wire_mesh(wire_mesh::TORUS, 24, 12, 0.5);
	r_b.translate(vecd3(0, -2, 0));
	
	object r_a_a;
	r_a_a.m = wire_mesh(wire_mesh::ICOSAHEDRON, 1);
	r_a_a.translate(vecd3(0, 0, -2));
	
	r_a.set_parent(&root);
	r_b.set_parent(&root);
	r_a_a.set_parent(&r_a);
	
	root.translate(vecd3(7, 0, 0));
	
	init_data id = {800, 800, 1, (char*) "Voxel", 0xFFFFFF, 0x0};
	event_map em;
	
	em.draw = draw;
	em.state = (void*) &root;
	
	app_handle ah;
	ah.mainloop(id, em);
	
	return 0;
}
