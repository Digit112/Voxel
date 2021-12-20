#ifndef sgl_render_group
#define sgl_render_group

namespace sgl {
	class render_group {
	public:
		// Children
		darray<render_group*> renders;
		darray<object*> objects;
		
		render_group();
		render_group(int o, int r);
		
		// Render propogation function (called by parent or, for the root node of the render tree, by the engine.)
		// This function wraps the function called below.
		// Note that this function will call the render_prop() of all children before calling its own render() function.
		// For this reason, all children will have constructed and returned expression trees.
		RGBAD render_prop(app_handle* ah, vecd3 origin, vecd3 dir, void* state, double delta_time);
		
		// User-provided render function.
		// Default render functions exist. Read documentation carefully.
		// They are often coupled with specific meshes and object render functions.
		// Mismatch = Compile Error / Memory Fault / Visual Artifacts
		RGBAD (*render)(app_handle* ah, vecd3 origin, vecd3 dir, void* state, double delta_time,
		                darray<RGBAD>& render_results, darray<RGBAD>& object_results);
	};
}

#include "sgl_render_group.cpp"

#endif
