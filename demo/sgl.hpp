// Include Utility library
#include "../Utility/util.hpp"
using namespace util;

namespace sgl {
	class app_handle;
	class init_data;
	class event;
	class event_map;

	class cam;
	
	class object;
	
	class render_module;
	
	class RGBAD {
	public:
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;
		float D;
		
		RGBAD() {}
		RGBAD(unsigned char R, unsigned char G, unsigned char B, unsigned char A, float D) : R(R), G(G), B(B), A(A), D(D) {}
	};
	
	class render_group;

	class wire_mesh;
	
	template<class T>
	class expr;
	
	void wireframe_render(app_handle* ah, cam* c, object* r, void* s, double dt);
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#include "../sgl_expr.hpp"
#include "../util.cpp"

#include "../sgl_mesh.hpp"
#include "../sgl_object.hpp"
#include "../sgl_render_group.hpp"
#include "../sgl_cam.hpp"
#include "../sgl.hpp"
#include "../default_renders.cpp"
#include "../sgl_collider.hpp"
