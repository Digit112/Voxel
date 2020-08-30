namespace sgl {
	cam::cam() {
		p = vecd3(0, 0, 0);
		r = quaternion(1, 0, 0, 0);
	}
	
	void cam::translate(vecd3 a, bool is_global = true) {
		if (is_global) {
			p = p + a;
			return;
		}
		p = p + r.apply(a);
	}
	
	void cam::rotate(quaternion a, bool is_global  = true) {
		if (is_global) {
			r = quaternion::hamilton(a, r);
			return;
		}
		r = quaternion::hamilton(r, a);
	}
	
	void cam::rotate(vecd3 axis, double theta, bool is_global = true) {
		if (is_global) {
			r = quaternion::hamilton(quaternion(axis, theta), r);
			return;
		}
		r = quaternion::hamilton(r, quaternion(axis, theta) );
	}
	
	object cam::apply(const object& a) {
		object o = a;
		o.p = (!r).apply(a.p - p);
		o.r = quaternion::hamilton(a.r, !r);
		return o;
	}
	
	void cam::apply(mesh_wire& a) {
		for (int i = 0; i < a.pn; i++) {
			a.p[i] = (!r).apply(a.p[i] - p);
		}
	}
}
