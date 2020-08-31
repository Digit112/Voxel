namespace sgl {
	/* vecd2 */
	vecd2::vecd2() : x(0), y(0) {}
	vecd2::vecd2(double x, double y) : x(x), y(y) {}
	
	double vecd2::mag() {
		return sqrt(x*x + y*y);
	}
	
	double vecd2::sqr_mag() {
		return x*x + y*y;
	}
	
	vecd2 vecd2::operator+(const vecd2& a) const {
		return vecd2(x+a.x, y+a.y);
	}
	vecd2 vecd2::operator-(const vecd2& a) const {
		return vecd2(x-a.x, y-a.y);
	}
	vecd2 vecd2::operator*(const vecd2& a) const {
		return vecd2(x*a.x, y*a.y);
	}
	vecd2 vecd2::operator/(const vecd2& a) const {
		return vecd2(x/a.x, y/a.y);
	}
	vecd2 vecd2::operator*(double a) {
		return vecd2(x*a, y*a);
	}
	vecd2 vecd2::operator/(double a) {
		return vecd2(x/a, y/a);
	}
	vecd2 vecd2::operator-() {
		return vecd2(-x, -y);
	}
	
	vecd2 vecd2::normalize() {
		double m = mag();
		return vecd2(x/m, y/m);
	}
	vecd2 vecd2::normalize(double t) {
		double m = mag() / t;
		return vecd2(x/m, y/m);
	}
	
	double vecd2::dot(vecd2 a, vecd2 b) {
		return a.x*b.x + a.y*b.y;
	}
	
	/* veci2 */
	veci2::veci2() : x(0), y(0) {}
	veci2::veci2(int x, int y) : x(x), y(y) {}
	
	double veci2::mag() {
		return sqrt(x*x + y*y);
	}
	
	int veci2::sqr_mag() {
		return x*x + y*y;
	}
	
	veci2 veci2::operator+(const veci2& a) const {
		return veci2(x+a.x, y+a.y);
	}
	veci2 veci2::operator-(const veci2& a) const {
		return veci2(x-a.x, y-a.y);
	}
	veci2 veci2::operator*(const veci2& a) const {
		return veci2(x*a.x, y*a.y);
	}
	veci2 veci2::operator/(const veci2& a) const {
		return veci2(x/a.x, y/a.y);
	}
	veci2 veci2::operator*(int a) {
		return veci2(x*a, y*a);
	}
	veci2 veci2::operator/(int a) {
		return veci2(x/a, y/a);
	}
	veci2 veci2::operator-() {
		return veci2(-x, -y);
	}
	
	vecd2 veci2::normalize() {
		double m = mag();
		return vecd2(x/m, y/m);
	}
	vecd2 veci2::normalize(double t) {
		double m = mag() / t;
		return vecd2(x/m, y/m);
	}
	
	int veci2::dot(vecd2 a, vecd2 b) {
		return a.x*b.x + a.y*b.y;
	}
	
	/* vecd3 */
	vecd3::vecd3() : x(0), y(0), z(0) {}
	vecd3::vecd3(double x, double y, double z) : x(x), y(y), z(z) {}
	
	double vecd3::mag() {
		return sqrt(x*x + y*y + z*z);
	}
	
	double vecd3::sqr_mag() {
		return x*x + y*y + z*z;
	}
	
	vecd3 vecd3::operator+(const vecd3& a) const {
		return vecd3(x+a.x, y+a.y, z+a.z);
	}
	vecd3 vecd3::operator-(const vecd3& a) const {
		return vecd3(x-a.x, y-a.y, z-a.z);
	}
	vecd3 vecd3::operator*(const vecd3& a) const {
		return vecd3(x*a.x, y*a.y, z*a.x);
	}
	vecd3 vecd3::operator/(const vecd3& a) const {
		return vecd3(x/a.x, y/a.y, z/a.z);
	}
	vecd3 vecd3::operator*(double a) {
		return vecd3(x*a, y*a, z*a);
	}
	vecd3 vecd3::operator/(double a) {
		return vecd3(x/a, y/a, z/a);
	}
	vecd3 vecd3::operator-() {
		return vecd3(-x, -y, -z);
	}
	
	vecd3 vecd3::normalize() {
		double m = mag();
		return vecd3(x/m, y/m, z/m);
	}
	vecd3 vecd3::normalize(double t) {
		double m = mag() / t;
		return vecd3(x/m, y/m, z/m);
	}
	
	double vecd3::dot(vecd3 a, vecd3 b) {
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	
	vecd3 vecd3::cross(vecd3 a, vecd3 b) {
		return vecd3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}
	
	/* veci3 */
	veci3::veci3() : x(0), y(0), z(0) {}
	veci3::veci3(int x, int y, int z) : x(x), y(y), z(z) {}
	
	double veci3::mag() {
		return sqrt(x*x + y*y + z*z);
	}
	
	int veci3::sqr_mag() {
		return x*x + y*y + z*z;
	}
	
	veci3 veci3::operator+(const veci3& a) const {
		return veci3(x+a.x, y+a.y, z+a.z);
	}
	veci3 veci3::operator-(const veci3& a) const {
		return veci3(x-a.x, y-a.y, z-a.z);
	}
	veci3 veci3::operator*(const veci3& a) const {
		return veci3(x*a.x, y*a.y, z*a.x);
	}
	veci3 veci3::operator/(const veci3& a) const {
		return veci3(x/a.x, y/a.y, z/a.z);
	}
	veci3 veci3::operator*(int a) {
		return veci3(x*a, y*a, z*a);
	}
	veci3 veci3::operator/(int a) {
		return veci3(x/a, y/a, z/a);
	}
	veci3 veci3::operator-() {
		return veci3(-x, -y, -z);
	}
	
	vecd3 veci3::normalize() {
		double m = mag();
		return vecd3(x/m, y/m, z/m);
	}
	vecd3 veci3::normalize(double t) {
		double m = mag() / t;
		return vecd3(x/m, y/m, z/m);
	}
	
	int veci3::dot(veci3 a, veci3 b) {
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	
	veci3 veci3::cross(veci3 a, veci3 b) {
		return veci3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}
	
	/* vecd4 */
	vecd4::vecd4() : w(0), x(0), y(0), z(0) {}
	vecd4::vecd4(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}
	
	double vecd4::mag() {
		return sqrt(w*w + x*x + y*y + z*z);
	}
	
	double vecd4::sqr_mag() {
		return w*w + x*x + y*y + z*z;
	}
	
	vecd4 vecd4::operator+(const vecd4& a) const {
		return vecd4(w+a.w, x+a.x, y+a.y, z+a.z);
	}
	vecd4 vecd4::operator-(const vecd4& a) const {
		return vecd4(w-a.w, x-a.x, y-a.y, z-a.z);
	}
	vecd4 vecd4::operator*(const vecd4& a) const {
		return vecd4(w*a.w, x*a.x, y*a.y, z*a.x);
	}
	vecd4 vecd4::operator/(const vecd4& a) const {
		return vecd4(w/a.w, x/a.x, y/a.y, z/a.z);
	}
	vecd4 vecd4::operator*(double a) {
		return vecd4(w*a, x*a, y*a, z*a);
	}
	vecd4 vecd4::operator/(double a) {
		return vecd4(w/a, x/a, y/a, z/a);
	}
	vecd4 vecd4::operator-() {
		return vecd4(-w, -x, -y, -z);
	}
	
	vecd4 vecd4::normalize() {
		double m = mag();
		return vecd4(w/m, x/m, y/m, z/m);
	}
	vecd4 vecd4::normalize(double t) {
		double m = mag() / t;
		return vecd4(w/m, x/m, y/m, z/m);
	}
	
	double vecd4::dot(vecd4 a, vecd4 b) {
		return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
	}
	
	/* veci4 */
	veci4::veci4() : w(0), x(0), y(0), z(0) {}
	veci4::veci4(int w, int x, int y, int z) : w(w), x(x), y(y), z(z) {}
	
	double veci4::mag() {
		return sqrt(w*w + x*x + y*y + z*z);
	}
	
	int veci4::sqr_mag() {
		return w*w + x*x + y*y + z*z;
	}
	
	veci4 veci4::operator+(const veci4& a) const {
		return veci4(w+a.w, x+a.x, y+a.y, z+a.z);
	}
	veci4 veci4::operator-(const veci4& a) const {
		return veci4(w-a.w, x-a.x, y-a.y, z-a.z);
	}
	veci4 veci4::operator*(const veci4& a) const {
		return veci4(w*a.w, x*a.x, y*a.y, z*a.x);
	}
	veci4 veci4::operator/(const veci4& a) const {
		return veci4(w/a.w, x/a.x, y/a.y, z/a.z);
	}
	veci4 veci4::operator*(int a) {
		return veci4(w*a, x*a, y*a, z*a);
	}
	veci4 veci4::operator/(int a) {
		return veci4(w/a, x/a, y/a, z/a);
	}
	veci4 veci4::operator-() {
		return veci4(-w, -x, -y, -z);
	}
	
	vecd4 veci4::normalize() {
		double m = mag();
		return vecd4(w / m, x / m, y / m, z / m);
	}
	vecd4 veci4::normalize(double t) {
		double m = mag() / t;
		return vecd4(w/m, x/m, y/m, z/m);
	}
	
	int veci4::dot(veci4 a, veci4 b) {
		return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
	}
	
	/* quaternion */
	quaternion::quaternion() : vecd4() {}
	quaternion::quaternion(double w, double x, double y, double z) : vecd4(w, x, y, z) {}

	quaternion::quaternion(vecd3 axis, double theta) {
		axis = axis.normalize();
		double s = sin(theta/2);
		w = cos(theta/2);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}
	
	quaternion quaternion::operator!() const {
		return quaternion(w, -x, -y, -z);
	}
	
	quaternion quaternion::hamilton(const quaternion& a, const quaternion& b) {
		return quaternion(a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z, a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y, a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x, a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w);
	}
	
	vecd3 quaternion::vhamilton(const quaternion& a, const quaternion& b) {
		return vecd3(a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y, a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x, a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w);
	}
	
	quaternion& quaternion::mhamilton(quaternion& a, const quaternion& b) {
		a.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
		a.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
		a.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
		a.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
		return a;
	}
	
	vecd3 quaternion::apply(const vecd3& in) const {
		return vhamilton(hamilton(*this, quaternion(0, in.x, in.y, in.z)), !*this);
	}
	
	vecd3 quaternion::rotate(vecd3 in, vecd3 axis_offset, vecd3 axis_dir, double theta) {
		in = in - axis_offset;
		in = quaternion(axis_dir, theta).apply(in);
		return in + axis_offset;
	}
}




















