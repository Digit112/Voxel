namespace sgl {
	object::object() : p(0, 0, 0), r(1, 0, 0, 0), s(1, 1, 1), children(), parent(NULL), color(0), is_hidden(false) {}
	object::object(int c) : p(0, 0, 0), r(1, 0, 0, 0), s(1, 1, 1), children(0, c), parent(NULL), color(0), is_hidden(false) {}
	
	void object::set_parent(object* p) {
		if (parent != NULL) {
			parent->children.remove(this);
		}
		parent = p;
		p->children.append(this);
	}
	
	void object::translate(vecd3 a, bool is_global = true) {
		if (is_global) {
			p = p + a;
		} else {
			p = p + r.apply(a);
		}
		
		for (int c = 0; c < this->children.size; c++) {
			this->children[c]->translate(a, is_global);
		}
	}
	
	void object::rotate(quaternion a, bool is_global = true) {
		if (is_global) {
			r = quaternion::hamilton(a, r);
		} else {
			r = quaternion::hamilton(r, a);
		}
		
		for (int c = 0; c < this->children.size; c++) {
			this->children[c]->rotate(this->p, a, is_global);
		}
	}
	
	void object::rotate(vecd3 offset, quaternion a, bool is_global = true) {
		if (is_global) {
			r = quaternion::hamilton(a, r);
			p = quaternion::rotate(p, offset, a);
		} else {
			r = quaternion::hamilton(r, a);
			p = quaternion::rotate(p, offset, quaternion::hamilton(r, a));
		}
		
		for (int c = 0; c < this->children.size; c++) {
			this->children[c]->rotate(offset, a, is_global);
		}
	}
	
	void object::rotate(vecd3 axis, double theta, bool is_global = true) {
		if (is_global) {
			r = quaternion::hamilton(quaternion(axis, theta), r);
		} else {
			r = quaternion::hamilton(r, quaternion(axis, theta) );
		}
		
		for (int c = 0; c < this->children.size; c++) {
			this->children[c]->rotate(this->p, axis, theta, is_global);
		}
	}
	
	void object::rotate(vecd3 offset, vecd3 axis, double theta, bool is_global = true) {
		if (is_global) {
			r = quaternion::hamilton(quaternion(axis, theta), r);
			p = quaternion::rotate(p, offset, axis, theta);
		} else {
			r = quaternion::hamilton(r, quaternion(axis, theta) );
			p = quaternion::rotate(p, offset, r.apply(axis), theta);
		}
		
		for (int c = 0; c < this->children.size; c++) {
			this->children[c]->rotate(offset, axis, theta, is_global);
		}
	}
	
	void object::scale(vecd3 a) {
		s = s * a;
		
		for (int c = 0; c < this->children.size; c++) {
			this->children[c]->scale(p, a);
		}
	}
	
	void object::scale(vecd3 offset, vecd3 a) {
		s = s * a;
		p = (p - offset) * a + offset;
		
		for (int c = 0; c < this->children.size; c++) {
			this->children[c]->scale(offset, a);
		}
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
	
	wire_mesh& object::applied(wire_mesh& o) const {
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
