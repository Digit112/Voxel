namespace sgl {
	render_group::render_group() : renders(0), objects(0), render(NULL) {}
	render_group::render_group(int o, int r) : renders(r), objects(o), render(NULL) {}
	
	RGBAD render_group::render_prop(app_handle* ah, vecd3 origin, vecd3 dir, void* state, double delta_time) {
		// Hold the results of child nodes
		darray<RGBAD> render_results(renders.size);
		darray<RGBAD> object_results(objects.size);
		
		// Propogate data to child render groups
		for (int i = 0; i < renders.size; i++) {
			render_results[i] = renders[i]->render_prop(ah, origin, dir, state, delta_time);
		}
		
		// Propogate data to child objects.
		for (int i = 0; i < objects.size; i++) {
			object_results[i] = objects[i]->render(ah, objects[i], origin, dir, state, delta_time);
		}
		
		// Now all children have returned expression trees.
		
		// Call custom render function.
		return render(ah, origin, dir, state, delta_time, render_results, object_results);
	}
}
