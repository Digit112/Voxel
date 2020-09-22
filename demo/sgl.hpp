namespace sgl {
	class app_handle;
	class init_data;
	class event;
	class event_map;

	class cam;
	
	class object;
	
	class render_module;

	class wire_mesh;

	class veci2;
	class vecd2;
	class veci3;
	class vecd3;
	class veci4;
	class vecd4;
	class quaternion;
	
	void wireframe_render(app_handle* ah, cam* c, object* r, void* s, double dt);
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <CL/cl.h>

#include "../darray.hpp"
#include "../util.cpp"
#include "../sgl_vec.hpp"

#include "../sgl_cam.hpp"
#include "../sgl.hpp"
#include "../sgl_mesh.hpp"
#include "../default_renders.cpp"
#include "../sgl_object.hpp"
#include "../sgl_collider.hpp"
