namespace sgl {
	pattern2::pattern2() : width(0), height(0), data(NULL) {}
	
	pattern2::pattern2(const pattern2& c) : width(c.width), height(c.height) {
		data = new double[width*height];
		for (int i = 0; i < width*height; i++) {
			data[i] = c.data[i];
		}
	}
	
	pattern2::pattern2(int width, int height) : width(width), height(height) {
		data = new double[width*height];
	}
	
	pattern2::pattern2(int width, int height, double fill) : width(width), height(height) {
		data = new double[width*height];
		for (int i = 0; i < width*height; i++) {
			data[i] = fill;
		}
	}
	
	void pattern2::operator=(const pattern2& c) {
		if (data != NULL) {
			delete[] data;
		}
		
		width = c.width;
		height = c.height;
		data = new double[width*height];
		for (int i = 0; i < width*height; i++) {
			data[i] = c.data[i];
		}
	}
	
	void pattern2::saveppm(const char* fn) {
		FILE* fp = fopen(fn, "wb+");
		
		char* buff = new char[256];
		sprintf(buff, "P5\n%d %d\n65535\n", width, height);
		fwrite(buff, 1, strlen(buff), fp);
		delete[] buff;
		
		unsigned short* out = new unsigned short[width*height];
		unsigned short temp;
		for (int i = 0; i < width*height; i++) {
			temp = floor(data[i] * 65535.9);
			// Reverse endianness
			out[i] = (temp << 8) | (temp >> 8);
		}
		fwrite(out, 2, width*height, fp);
		delete[] out;
		
		fclose(fp);
	}
	
	void pattern2::fill(double val) {
		for (int i = 0; i < width*height; i++) {
			data[i] = val;
		}
	}
	
	void pattern2::paint(pattern2 p, float opacity, int x, int y) {
		int Mx = x + p.width;
		if (width < Mx) Mx = width;
		int My = y + p.height;
		if (height < My) My = height;
		
		float myop = 1 - opacity;
		
		int x0; int x1; int y0; int y1; int rowc0; int rowc1;
		for (y0 = 0, y1 = y; y1 < My; y1++) {
			rowc0 = p.width*y0;
			rowc1 = width*y1;
			
			for (x0 = 0, x1 = x; x1 < Mx; x1++) {
				data[rowc1+x1] = myop*data[rowc1+x1] + opacity*p.data[rowc0+x0];
				x0++;
			}
			y0++;
		}
	}
	
	void pattern2::scale(pattern2 p, int x, int y) {
		int Mx = x + p.width;
		if (width < Mx) Mx = width;
		int My = y + p.height;
		if (height < My) My = height;
		
		int x0; int x1; int y0; int y1; int rowc0; int rowc1;
		for (y0 = 0, y1 = y; y1 < My; y1++) {
			rowc0 = p.width*y0;
			rowc1 = width*y1;
			
			for (x0 = 0, x1 = x; x1 < Mx; x1++) {
				data[rowc1+x1] = data[rowc1+x1] * p.data[rowc0+x0];
				x0++;
			}
			y0++;
		}
	}
	
	void pattern2::fit_values() {
		double min = 1;
		double max = 0;
		
		for (int i = 0; i < width*height; i++) {
			min = fmin(data[i], min);
			max = fmax(data[i], max);
		}
		
		
		double scale = 1/(max-min);
		printf("Max: %.2f, Min: %.2f, Scale: %.2f\n", max, min, scale);
		for (int i = 0; i < width*height; i++) {
			data[i] = (data[i] - min) * scale;
		}
	}	
	
	pattern2::~pattern2() {
		if (data != NULL) {
			delete[] data;
		}
	}
	
	procedural::procedural() : buf1l(0), buf1(NULL), buf2w(0), buf2h(0), buf2(NULL) {}
	
	unsigned int procedural::FNV_32(unsigned int seed) {
		unsigned int hash = 2166136261;
		for (int i = 0; i < 4; i++) {
			hash *= 16777619;
			hash ^= (unsigned char) (seed >> (i*8));
		}
		return hash;
	}
	
	unsigned int procedural::FNV_32(unsigned long seed) {
		unsigned int hash = 2166136261;
		for (int i = 0; i < 8; i++) {
			hash *= 16777619;
			hash ^= (unsigned char) (seed >> (i*8));
		}
		return hash;
	}
	
	// Expects null-terminated string.
	unsigned int procedural::FNV_32(const char* seed) {
		unsigned int hash = 2166136261;
		for (int i = 0; seed[i] != '\0'; i++) {
			hash *= 16777619;
			hash ^= seed[i];
		}
		return hash;
	}
	
	void procedural::init(unsigned int sd) {
		seed[0] = FNV_32(sd);
		seed[1] = FNV_32(seed[0]);
		seed[2] = FNV_32(seed[1]);
		seed[3] = FNV_32(seed[2]);
	}
	
	// Converted to C++
	int procedural::random_int() {
		/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

		To the extent possible under law, the author has dedicated all copyright
		and related and neighboring rights to this software to the public domain
		worldwide. This software is distributed without any warranty.

		See <http://creativecommons.org/publicdomain/zero/1.0/>
		Original <http://xoshiro.di.unimi.it/xoshiro128starstar.c>  */
		
		unsigned int temp = seed[0] * 5;
		unsigned int result = (temp << 7) | (temp >> 25);
		
		unsigned int t = seed[1] << 9;
		
		seed[2] ^= seed[0];
		seed[3] ^= seed[1];
		seed[1] ^= seed[2];
		seed[0] ^= seed[3];
		
		temp = seed[3];
		seed[3] = (temp << 11) | (temp >> 21);
		
		return (int) result;
	}
	
	double procedural::random_val() {
		return (double) random_int() / 0xFFFFFFFF + 0.5;
	}
	
	pattern2& procedural::sin_wave(pattern2& p, int x, int y, int wavelength, float offset) {
		float xd; float yd; float dis;
		int rowc;
		for (int py = 0; py < p.height; py++) {
			yd = py - y;
			yd *= yd;
			
			rowc = py * p.width;
			for (int px = 0; px < p.width; px++) {
				xd = px - x;
				xd *= xd;
				
				dis = sqrt(xd + yd);
				p.data[px+rowc] = sin(2*3.14159*(dis / wavelength + offset)) / 2 + 0.5;
			}
		}
				
		return p;		
	}
	
	pattern2& procedural::log_circle(pattern2& p, int x, int y, int r) {
		float xd; float yd; float dis;
		int rowc;
		for (int py = 0; py < p.height; py++) {
			yd = py - y;
			yd *= yd;
			
			rowc = py * p.width;
			for (int px = 0; px < p.width; px++) {
				xd = px - x;
				xd *= xd;
				
				dis = sqrt(xd + yd);
				p.data[px+rowc] = fmax((log10(r-dis)+1) / (log10(r)+1), 0);
			}
		}
				
		return p;		
	}
	
	pattern2& procedural::ln_circle(pattern2& p, int x, int y, int r) {
		float xd; float yd; float dis;
		int rowc;
		for (int py = 0; py < p.height; py++) {
			yd = py - y;
			yd *= yd;
			
			rowc = py * p.width;
			for (int px = 0; px < p.width; px++) {
				xd = px - x;
				xd *= xd;
				
				dis = sqrt(xd + yd);
				p.data[px+rowc] = fmax((log(r-dis)+1) / (log(r)+1), 0);
			}
		}
				
		return p;		
	}
	
	pattern2& procedural::lin_circle(pattern2& p, int x, int y, int r) {
		float xd; float yd; float dis;
		int rowc;
		for (int py = 0; py < p.height; py++) {
			yd = py - y;
			yd *= yd;
			
			rowc = py * p.width;
			for (int px = 0; px < p.width; px++) {
				xd = px - x;
				xd *= xd;
				
				dis = sqrt(xd + yd);
				p.data[px+rowc] = fmax(-dis/r+1, 0);
			}
		}
				
		return p;		
	}
	
	pattern2& procedural::logistic_circle(pattern2& p, int x, int y, int r) {
		float xd; float yd; float dis;
		int rowc;
		for (int py = 0; py < p.height; py++) {
			yd = py - y;
			yd *= yd;
			
			rowc = py * p.width;
			for (int px = 0; px < p.width; px++) {
				xd = px - x;
				xd *= xd;
				
				dis = sqrt(xd + yd);
				p.data[px+rowc] = -2 / (1 + pow(2.718282, -dis * 5 / r)) + 2;
			}
		}
				
		return p;		
	}
	
	pattern2& procedural::simplex(pattern2& p, double maxX, double maxY, double minX, double minY) {
		// Working variables
		double s;
		double z;
		
		unsigned int sd = (unsigned int) random_int();
		
		// Check order of min/max
		if (maxX < minX) {
			s = minX;
			minX = maxX;
			maxX = s;
		}
		if (maxY < minY) {
			s = minY;
			minY = maxY;
			maxY = s;
		}
		
		// Variables to store the original specified region.
		double mx = minX;
		double Mx = maxX;
		double my = minY;
		double My = maxY;
		
		// Expand the region provided by the player to include all corners which have any influence over any point in the original region.
		minX--;
		minY--;
		maxX++;
		maxY++;
		
//		printf("Region (%.2f, %.2f) - (%.2f, %.2f) Expanded into (%.2f, %.2f) - (%.2f, %.2f)\n", mx, my, Mx, My, minX, minY, maxX, maxY);
		
		// Perform the inverse equalateralization transform to the upper left and lower right corners of the supplied region.
		s = 0.366025404 * (minX + maxY);
		double high_left_x = minX + s;
		double high_left_y = maxY + s;
		s = 0.366025404 * (maxX + minY);
		double low_right_x = maxX + s;
		double low_right_y = minY + s;
		
		// Perform the transform on the remaining corners as well, although they don't need new variables.
		s = 0.366025404 * (minX + minY);
		minX += s;
		minY += s;
		s = 0.366025404 * (maxX + maxY);
		maxX += s;
		maxY += s;
		
//		printf("Corners become (ABCD): (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", minX, minY, high_left_x, high_left_y, maxX, maxY, low_right_x, low_right_y);
		
		// Cache useful variables.
		int intMinX = (int) ceil(minX);
		int intMaxX = (int) ceil(maxX);
		
		// Divide the newly-defined parallelogram into three segments arranged horizontally, two triangles and one parallelogram in the middle.
		
		// In cases where the sampled region has a low enough aspect ratio, the high_left_x might be farther right than the low_right_x value.
		// In this case we must ensure the regions are re-ordered correctly.
		double first_switch;
		double last_switch;
		if (high_left_x < low_right_x) {
			first_switch = high_left_x;
			last_switch = low_right_x;
		} else {
//			printf("Swapping switch points.\n");
			first_switch = low_right_x;
			last_switch = high_left_x;
		}
		
//		printf("Switches at %.2f & %.2f\n", first_switch, last_switch);
			
		// Assign one of four random vectors to each lattice point (point having integer coordinates) contained in the parallelogram.
		// First Iterate over integer x values from the left-most point in the parallelogram to the right-most point.
		// For each x, iterate over all y values such that (x, y) is a point in the parallelogram.
		
		// buffer2 contains the randomly-chosen vector for all lattice points with influence over the supplied region.
		
		// Important: For some n, buffer1[n] gives the y value of the lattice point whose assigned vector is given at buffer2[n][0].
		// The y-value of the lattice point whose vector is given at buffer[n][a] is one greater than the y-value for buffer[n][a-1]
		
		s = 0.267949192f;
		z = 3.732050808f;
		
		// Allocate space for a jagged matrix.
		int num_cols = intMaxX - intMinX;
		unsigned char** buffer2 = new unsigned char*[num_cols];
		int*   buffer1 = new int[num_cols];
		
//		printf("Allocating for %d columns in range %.2f - %.2f\n", num_cols, minX, maxX);
		
		unsigned long col_seed;
		unsigned long pix_seed;
		int I;
		int min_y;
		int max_y;
		for (int x = intMinX; x < maxX; x++) {
//			printf("Column %d\n", x);
			I = x - intMinX;
			col_seed = x ^ sd;
			col_seed <<= 32;
			
			if (x < first_switch) {
				// Allocate space for this row.
				min_y = ceil((x - minX) * s + minY);
				max_y = floor((x - minX) * z + minY);
				
//				printf("buffer2[%d] gets %d - %d + 1.\n", I, max_y, min_y);
				
				buffer2[I] = new unsigned char[max_y - min_y + 1];
				buffer1[I] = min_y;
				
				// Iterate over y values.
				for (int y = min_y; y <= max_y; y++) {
//					printf("Row 0 / %d\n", y);
					unsigned int a = y ^ sd;
					pix_seed = col_seed | y;
					init(FNV_32(pix_seed));
					buffer2[I][y-min_y] = abs(random_int() % 4);
				}
			}
			else if (x < last_switch) {
				// Allocate space for this row.
				min_y = ceil((x - minX) * s + minY);
				max_y = floor((x - maxX) * s + maxY);
				
//				printf("buffer2[%d] gets %d - %d + 1.\n", I, max_y, min_y);
				
				buffer2[I] = new unsigned char[max_y - min_y + 1];
				buffer1[I] = min_y;
				
				// Iterate over y values.
				for (int y = min_y; y <= max_y; y++) {
//					printf("Row 1 / %d\n", y);
					pix_seed = col_seed | y;
					init(FNV_32(pix_seed));
					buffer2[I][y-min_y] = abs(random_int() % 4);
				}
			}
			else {
				// Allocate space for this row.
				min_y = ceil((x - maxX) * z + maxY);
				max_y = floor((x - maxX) * s + maxY);
				
//				printf("buffer2[%d] gets %d - %d + 1.\n", I, max_y, min_y);
				
				buffer2[I] = new unsigned char[max_y - min_y + 1];
				buffer1[I] = min_y;
				
				// Iterate over y values.
				for (int y = min_y; y <= max_y; y++) {
//					printf("Row 2 / %d\n", y);
					pix_seed = col_seed | y;
					init(FNV_32(pix_seed));
					buffer2[I][y-min_y] = abs(random_int() % 4);
				}
			}
		}
		
//		printf("Vector Assignment Complete.\n");
		
		// Now buffer2 contains a random vector for every relevant lattice point.
		// Now we iterate over every point in the supplied region to calculate the simplex noise at each point.
		// Store the 3corners of the simplex that contains point p and their values.
		vecd2 px; vecd2 py; vecd2 pz;
		vecd2 vx; vecd2 vy; vecd2 vz;
		
		// Ugh
		double left; double low; double right; double high;
		double u; double v; double w; double m;
		double X; double Y; double Px; double Py;
		vecd2 Uvec; vecd2 Vvec; vecd2 Wvec;
		for (int x = 0; x < p.width; x++) {
			for (int y = 0; y < p.height; y++) {
				// Get input point coordinates
				Px = ((double) x / p.width) * (Mx - mx) + mx;
				Py = ((double) y / p.height) * (My - my) + my;
				
				// Apply the equalateralization transform to the input point
				s = 0.3660254037f * (Px + Py);
				X = Px + s;
				Y = Py + s;
				
//				printf("\n(%d, %d) -> (%.2f, %.2f) -> (%.2f, %.2f)\n", x, y, Px, Py, X, Y);
				
				// Get the corners of the square in which (X, Y) resides.
				left = floor(X);
				low = floor(Y);
				right = floor(X+1);
				high = floor(Y+1);
				
//				printf("Bounded by %.2f < x < %.2f & %.2f < y < %.2f.\n", left, right, low, high);
				
				// In quadrants 2 and 4, due to one value being negative, either (X % 1) is always greater than (Y % 1) or vice verse. This code corrects that by making negatives positve.
				if (X < 0) {
					X = fmodf(X, 1) + 1;
				}
				if (Y < 0) {
					Y = fmodf(Y, 1) + 1;
				}
				
				// Get the two corners that the point must belong to.
				px = vecd2(left, low);
				py = vecd2(right, high);
				// Get the last corner.
				if (fmodf(X, 1) > fmodf(Y, 1)) {
					pz = vecd2(right, low);
				} else {
					pz = vecd2(left, high);
				}
				
//				printf("Bounding half-square is: (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", px.x, px.y, py.x, py.y, pz.x, pz.y);
				
				// Get the random vectors stored in buffer2
				// Convert (x, y) coords of each point to (u, v) buffer2 coordinates.
				u = px.x - intMinX;
				v = px.y - (double) buffer1[(int) u];
//				printf("px -> uv: (%.2f, %.2f), (%.2f, %.2f)\n", px.x, px.y, u, v);
				vx = vecpick2[buffer2[(int) u][(int) v]];
				u = py.x - intMinX;
				v = py.y - (double) buffer1[(int) u];
//				printf("py -> uv: (%.2f, %.2f), (%.2f, %.2f)\n", py.x, py.y, u, v);
				vy = vecpick2[buffer2[(int) u][(int) v]];
				u = pz.x - intMinX;
				v = pz.y - (double) buffer1[(int) u];
//				printf("pz -> uv: (%.2f, %.2f), (%.2f, %.2f)\n", pz.x, pz.y, u, v);
				vz = vecpick2[buffer2[(int) u][(int) v]];
				
				// Skew the corners of this simplex into a simplex grid.
				s = 0.2113248654f * (px.x + px.y);
				px = vecd2(px.x - s, px.y - s);
				
				s = 0.2113248654f * (py.x + py.y);
				py = vecd2(py.x - s, py.y - s);
				
				s = 0.2113248654f * (pz.x + pz.y);
				pz = vecd2(pz.x - s, pz.y - s);
				
//				printf("Bounding simplex is: (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", px.x, px.y, py.x, py.y, pz.x, pz.y);
				
				Uvec = vecd2(Px - px.x, Py - px.y);
				Vvec = vecd2(Px - py.x, Py - py.y);
				Wvec = vecd2(Px - pz.x, Py - pz.y);
				
				// Calculate influence of each corner to the given point by distance.
				u = 2 * fmin(Uvec.sqr_mag(), 0.5);
				v = 2 * fmin(Vvec.sqr_mag(), 0.5);
				w = 2 * fmin(Wvec.sqr_mag(), 0.5);
				
//				printf("Corner-Distance vectors are (%.2f, %.2f) @ %.2f, (%.2f, %.2f) @ %.2f, (%.2f, %.2f) @ %.2f\n", Uvec.x, Uvec.y, u, Vvec.x, Vvec.y, v, Wvec.x, Wvec.y, w);
				
				// Modify values to simulate a quadratice falloff of each corner's influence
				u = u * (u - 1) - u + 1;
				v = v * (v - 1) - v + 1;
				w = w * (w - 1) - w + 1;
				
				// Normalize u, v, and w so they sum to 1
				m = u + v + w;
				u = u / m; v = v / m; w = w / m;
				
//				printf("Corner-Distance vectors are (%.2f, %.2f) @ %.2f, (%.2f, %.2f) @ %.2f, (%.2f, %.2f) @ %.2f\n", Uvec.x, Uvec.y, u, Vvec.x, Vvec.y, v, Wvec.x, Wvec.y, w);
//				printf("Randomly Assigned vectors are (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", vx.x, vx.y, vy.x, vy.y, vz.x, vz.y);
				
				// Get the dot product of the vector pointing from each corner toward the point, and the corner's randomly assigned vector. Then multiply that contribution by this point's influence.
				u *= vecd2::dot(Uvec, vx);
				v *= vecd2::dot(Vvec, vy);
				w *= vecd2::dot(Wvec, vz);
				
				p.data[x + y*p.width] = (u + v + w + 1) / 2;
				
//				printf("Final weights are %.2f, %.2f, %.2f for a value of %f\n", u, v, w, p.data[x+y*p.width]);
			}
		}
		
		delete[] buffer1;
		for (int i = 0; i < num_cols; i++) {
			delete[] buffer2[i];
		}
		delete[] buffer2;
		
		return p;
	}
	
	pattern2& procedural::worley(pattern2& p, double maxX, double maxY, double minX, double minY) {
		unsigned int sd = random_int();
		
		// Ensure the min/max values are in the right order
		double s;
		if (minX > maxX) {
			s = minX;
			minX = maxX;
			maxX = s;
		}
		if (minY > maxY) {
			s = minY;
			minY = maxY;
			maxY = s;
		}

		// Store original specified region
		double mx = minX;
		double Mx = maxX;
		double my = minY;
		double My = maxY;

		// Adjust region outward
		minX = floor(minX) - 1;
		maxX = ceil(maxX) + 1;
		minY = floor(minY) - 1;
		maxY = ceil(maxY) + 1;
		
		// Populate a 2D buffer with a random Vector between (0, 0) and (1, 1) for each square in the region.
		int vec_w = (int) (maxX - minX);
		int vec_h = (int) (maxY - minY);
		vecd2* pb = new vecd2[ vec_w*vec_h ];
		vecd2* pbr;
		
		unsigned long row_seed;
		int ix; int iy;
		for (int y = (int) minX; y < My; y++) {
			row_seed = y ^ sd;
			row_seed <<= 32;
			
			iy = (int) (y - minY);
			
			pbr = pb + iy*vec_w;
			for (int x = (int) minX; x < Mx; x++) {
				init(FNV_32(row_seed | (x ^ sd)));
				
				ix = (int) (x - minX);
				
				pbr[ix] = vecd2(random_val(), random_val());
				
				printf("Assigned (%.2f, %.2f) to (%d, %d)\n", pbr[ix].x, pbr[ix].y, x, y);
			}	
		}
		
		double X; double Y;
		double A; double B;
		double m;
		double searchMinX; double searchMinY; double searchMaxX; double searchMaxY;
		
		double tempVecX; double tempVecY;
		
		for (int y = 0; y < p.height; y++) {
			for (int x = 0; x < p.width; x++) {
				// Calculate position of this point in worley-space
				X = (double) x / p.width  * (Mx - mx) + mx;
				Y = (double) y / p.height * (My - my) + my;
				
//				printf("(%d, %d) -> (%.2f, %.2f) -> ", x, y, X, Y);
				
				// Get coordinates of upper right and lower left grid squares to search.
				searchMinX = floor(X) - 1;
				searchMinY = floor(Y) - 1;
				searchMaxX = searchMinX + 3;
				searchMaxY = searchMinY + 3;
				
				// Modify X and Y to be in terms of the 3x3 grid square space;
				X -= searchMinX;
				Y -= searchMinY;
				
//				printf("(%.2f, %.2f) searches (%.1f, %.1f) - (%.1f, %.1f)\n", X, Y, searchMinX, searchMinY, searchMaxX, searchMaxY);
				
				s = 2;
				// Iterate over 9 relevant points to find which is the closest to the current point.
				for (int b = searchMinY; b < searchMaxY; b++) {
					pbr = pb + (b - (int) minY)*vec_w;
					for (int a = searchMinX; a < searchMaxX; a++) {
						A = a - searchMinX;
						B = b - searchMinY;
						
						tempVecX = pbr[a - (int) minX].x + A - X;
						tempVecY = pbr[a - (int) minX].y + B - Y;
						
						m = tempVecX * tempVecX + tempVecY * tempVecY;
						
						if (m < s) {
							s = m;
						}
					}
				}
				
				// Now s should contain the square distance from this point to the nearest point.
				p.data[x+y*p.width] = sqrt(s) / 1.41;
			}
		}
		
		delete[] pb;
		
		return p;
	}
	
	pattern2& procedural::band(pattern2& p, double inc) {
		for (int i = 0; i < p.width*p.height; i++) {
			p.data[i] = round(p.data[i] / inc) * inc;
		}
		
		return p;
	}
	
	procedural::~procedural() {
		if (buf1 != NULL) {
			delete[] buf1;
		}
		if (buf2 != NULL) {
			delete[] buf2;
		}
	}
}

