#include <stdio.h>

#include "sgl.hpp"

using namespace sgl;

int main() {
	app_handle ah;
	
	object o;
	wire_mesh m(sgl::wire_mesh::CUBE);
	o.m = m;
	
	vecd3 ori(0, 0, 2);
	vecd3 dir(1, 1, -2);
	
	RGBAD r = wire_mesh_renderer(&ah, &o, ori, dir, NULL, 0).evaluate();
	
	printf("(%u, %u, %u, %u) %f\n", r.R, r.G, r.B, r.A, r.D);
	
	return 0;
}
