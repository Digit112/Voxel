namespace sgl {
	object::object() : p(0, 0, 0), r(1, 0, 0, 0), s(1, 1, 1), color(0), is_hidden(false) {}
	
	void object::translate(vecd3 a, bool is_global = true) {
		if (is_global) {
			p = p + a;
			return;
		}
		p = p + r.apply(a);
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
	
	vecd3 object::forward() {
		return r.apply(vecd3(1, 0, 0));
	}
	vecd3 object::right() {
		return r.apply(vecd3(0, 1, 0));
	}
	vecd3 object::up() {
		return r.apply(vecd3(0, 0, 1));
	}
	
	void object::hide() {
		is_hidden = true;
	}
	
	void object::show() {
		is_hidden = false;
	}
	
	mesh_wire& object::applied(mesh_wire& o) const {
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
