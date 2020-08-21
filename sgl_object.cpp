namespace sgl {
	object::object() {
		p = vecd3(0, 0, 0);
		r = quaternion(1, 0, 0, 0);
		s = vecd3(1, 1, 1);
	}
	
	void object::translate(vecd3 a) {
		p = p + a;
	}
	
	void object::rotate(quaternion a, bool is_global = true) {
		if (is_global) {
			r = quaternion::hamilton(a, r);
			return;
		}
		r = quaternion::hamilton(r, a);
	}
	
	void object::rotate(vecd3 axis, double theta, bool is_global = true) {
		if (is_global) {
			r = quaternion::hamilton(quaternion(axis, theta), r);
			return;
		}
		r = quaternion::hamilton(r, quaternion(axis, theta) );
	}
	
	void object::scale(vecd3 a) {
		s = s * a;
	}
	
	mesh_wire& object::applied(mesh_wire& o) {
		o.pn = m.pn;
		o.en = m.en;
		o.p = darray<vecd3>(o.pn);
		o.e = darray<veci2>(o.en);
		for (int i = 0; i < o.pn; i++) {
			o.p[i] = r.apply(m.p[i] * s) + p;
		}
		for (int i = 0; i < o.en; i++) {
			o.e[i] = m.e[i];
		}
		return o;
	}
}
