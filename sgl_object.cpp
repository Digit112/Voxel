namespace sgl {
	void object::translate(vecd3 a) {
		p = p + a;
	}
	
	void object::rotate(quaternion a) {
		r = quaternion::hamilton(r, a);
	}
	
	void object::scale(vecd3 a) {
		s = s * a;
	}
}
