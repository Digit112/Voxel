#ifndef util_procedural
#define util_procedural

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

namespace sgl {
	// simple class for holding 2D monochrome data.
	class pattern2 {
	public:
		int width;
		int height;
		
		double* data;
		
		pattern2();
		pattern2(const pattern2&);
		pattern2(int width, int height);
		pattern2(int width, int height, double fill);
		
		void operator=(const pattern2&);
		
		void saveppm(const char* fn);
		
		void fill(double val);
		
		// Copy the supplied pattern over this pattern, aligning the upper left corner to x and y and with the given opacity ranging from 0 to 1.
		void paint(pattern2 p, float opacity = 1, int x = 0, int y = 0);
		
		// Same as paint() but multiply values instead of averaging them.
		void scale(pattern2 p, int x = 0, int y = 0);
		
		// Scale all the values in this pattern such that the lowest becomes 0 and the highest becomes 1
		void fit_values();
		
		~pattern2();
	};
	
	class procedural {
	public:
		// RNG seed
		unsigned int seed[4] = {0, 0, 0, 0};
		
		// Preset vector choices for simplex noise generation
		const vecd2 vecpick2[4] = {vecd2(1, 1), vecd2(1, -1), vecd2(-1, 1), vecd2(-1, -1)};
		const vecd3 vecpick3[12];
		
		// Buffers
		int buf1l;
		double* buf1;
		int buf2w;
		int buf2h;
		double* buf2;
		
		procedural();
		
		// FNV_32 hash function
		unsigned int FNV_32(unsigned int seed);
		unsigned int FNV_32(unsigned long seed);
		unsigned int FNV_32(const char* seed);
		
		// RNG initialization
		void init(unsigned int sd = 0);
		
		// RNG functions
		int random_int();
		double random_val();
		
		/* Simple patterns. All coordinates and distances are given in pixels. */
		
		// Generate a sin wave emanating from a point.
		pattern2& sin_wave(pattern2& p, int x, int y, int wavelength, float offset = 0);
		
		// Generate a circle around a point where the brightness of each pixel falls off with the common log
		pattern2& log_circle(pattern2& p, int x, int y, int r);
		
		// Same as log_circle, but using the natural log.
		pattern2& ln_circle(pattern2& p, int x, int y, int r);
		
		// Generates a circle where brightness falls off with linear distance from the circle's center.
		pattern2& lin_circle(pattern2& p, int x, int y, int r);
		
		// Creates a circle where brightness falls off according to a logistic curve.
		// This circle never technically ends, but by the time it reaches r pixels it will be pretty dark.
		// Adjust the r as necessary.
		pattern2& logistic_circle(pattern2& p, int x, int y, int r);
		
		/* Random Noise */
		
		// 2D Simplex Noise
		pattern2& simplex(pattern2& p, double maxX, double maxY, double minX = 0, double minY = 0);
		
		// 2D Worley Noise
		pattern2& worley(pattern2& p, double maxX, double maxY, double minX = 0, double minY = 0);
		
		/* Operations */ 
		
		// Rounds the values of all pixels towards the nearest multiple of "inc".
		pattern2& band(pattern2& p, double inc);
		
		~procedural();
	};
}

#include "sgl_procedural.cpp"

#endif
