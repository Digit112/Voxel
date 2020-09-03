namespace sgl {
	vecd3 box_collider::raycast(vecd3 p, quaternion r, vecd3 s, vecd3 origin, vecd3 dir) {
		// Create local copies of member variables for scaling
		double _xp = xp*s.x; double _xn = xn*s.x; double _yp = yp*s.y; double _yn = yn*s.y; double _zp = zp*s.z; double _zn = zn*s.z;
		origin = origin - p;
		origin = (!r).apply(origin);
		dir = (!r).apply(dir);
		
		vecd3 nan_vec = vecd3(NAN, NAN, NAN);
		
		vecd3 clip_pt_x = nan_vec;
		vecd3 clip_pt_y = nan_vec;
		vecd3 clip_pt_z = nan_vec;
		
		// Whether to intersect with +x or -x
		if (dir.x < 0 && origin.x > _xp) {
			// +x
			clip_pt_x = dir / dir.x * (_xp - origin.x) + origin;
		}
		else if (dir.x > 0 && origin.x < _xn) {
			// -x
			clip_pt_x = dir / dir.x * (_xn - origin.x) + origin;
		}
		// Check whether the collision occured within the face of the collider
		if (clip_pt_x.y > _yp || clip_pt_x.y < _yn || clip_pt_x.z > _zp || clip_pt_x.z < _zn) {
			clip_pt_x = nan_vec;
		}
		// Whether to intersect with +y or -y
		if (dir.y < 0 && origin.y > _yp) {
			// +y
			clip_pt_y = dir / dir.y * (_yp - origin.y) + origin;
		}
		else if (dir.y > 0 && origin.y < _yn) {
			// -y
			clip_pt_y = dir / dir.y * (_yn - origin.y) + origin;
		}
		if (clip_pt_y.x > _xp || clip_pt_y.x < _xn || clip_pt_y.z > _zp || clip_pt_y.z < _zn) {
			clip_pt_y = nan_vec;
		}
		// Whether to intersect with +z or -z
		if (dir.z < 0 && origin.z > _zp) {
			// +z
			clip_pt_z = dir / dir.z * (_zp - origin.z) + origin;
		}
		else if (dir.z > 0 && origin.z < _zn) {
			// -z
			clip_pt_z = dir / dir.z * (_zn - origin.z) + origin;
		}
		if (clip_pt_z.y > _yp || clip_pt_z.y < _yn || clip_pt_z.x > _xp || clip_pt_z.x < _xn) {
			clip_pt_z = nan_vec;
		}
		
		// Figure out which collisions are valid and invalid, return nan_vec if no collision occured,
		// return the valid vector if there is only one, and if two valid vectors exist return the vector of lesser magnitude as its
		// collision occured first.
		if (clip_pt_x.is_nan()) {
			if (clip_pt_y.is_nan()) {
				if (clip_pt_z.is_nan()) {
					return nan_vec;
				} else {
					clip_pt_z = r.apply(clip_pt_z) + p;
					return clip_pt_z;
				}
			}
			clip_pt_y = r.apply(clip_pt_y) + p;
			if (clip_pt_z.is_nan()) {
				return clip_pt_y;
			} else {
				return clip_pt_y.sqr_mag() < clip_pt_z.sqr_mag() ? clip_pt_y : clip_pt_z;
			}
		}
		clip_pt_x = r.apply(clip_pt_x) + p;
		if (clip_pt_y.is_nan()) {
			if (clip_pt_z.is_nan()) {
				return clip_pt_x;
			} else {
				return clip_pt_x.sqr_mag() < clip_pt_z.sqr_mag() ? clip_pt_x : clip_pt_z;
			}
		}
		if (clip_pt_z.is_nan()) {
			return clip_pt_x.sqr_mag() < clip_pt_y.sqr_mag() ? clip_pt_x : clip_pt_y;
		}
	}
}
