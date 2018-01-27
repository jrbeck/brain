#ifndef V2D_H_
#define V2D_H_

#include <math.h>


// typedefs * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
typedef struct {
	double x;
	double y;
} v2d_t;

typedef struct {
	int x;
	int y;
} v2d_int;


// prototypes * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int v2d_isequal (v2d_t a, v2d_t b);
v2d_t v2d_zero (void);
void v2d_zero (v2d_t *a);
v2d_t v2d_unit (double theta);
v2d_t v2d_vector (double x, double y);
double v2d_dist (v2d_t a, v2d_t b);
double v2d_mag (v2d_t a);
v2d_t v2d_normalize (v2d_t a);
v2d_t v2d_scale (v2d_t a, double scalar);
v2d_t v2d_add (v2d_t a, v2d_t b);
v2d_t v2d_sub (v2d_t a, v2d_t b);
double v2d_dot (v2d_t a, v2d_t b);
double v2d_cross (v2d_t a, v2d_t b);
v2d_t v2d_rot (v2d_t a, double theta);
v2d_t v2d_neg (v2d_t a);
v2d_t v2d_normal (v2d_t a);
double v2d_theta (v2d_t a);
v2d_t v2d_project (v2d_t a, v2d_t b);
v2d_t v2d_interpolate (v2d_t a, v2d_t b, double percent);


// functions * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// return 1 if equal, 0 otherwise
int v2d_isequal (v2d_t a, v2d_t b) {
	if (a.x == b.x && a.y == b.y) return 1;
	return 0;
}


// returns a vector of magnitude zero
v2d_t v2d_zero (void) {
	v2d_t a;

	a.x = 0.0f;
	a.y = 0.0f;

	return a;
}


// zeroes out an existing vector
void v2d_zero (v2d_t *a) {
	a->x = 0.0f;
	a->y = 0.0f;
}


// returns unit vector at angle theta
v2d_t v2d_unit (double theta) {
	v2d_t a;

	a.x = cos (theta);
	a.y = sin (theta);

	return a;
}


// returns a vector
v2d_t v2d_vector (double x, double y) {
	v2d_t a;

	a.x = x;
	a.y = y;

	return a;
}


// returns the distance between two points
double v2d_dist (v2d_t a, v2d_t b) {
	a.x -= b.x;
	a.y -= b.y;

	return (sqrt ((a.x * a.x) + (a.y * a.y)));
}


// returns the magnitude of the vector
double v2d_mag (v2d_t a) {
	return (sqrt ((a.x * a.x) + (a.y * a.y)));
}


// returns the unit vector in the same direction as a
v2d_t v2d_normalize (v2d_t a) {
	double len = sqrt ((a.x * a.x) + (a.y * a.y));

	if (len <= 0.0f) return a;

	a.x /= len;
	a.y /= len;

	return a;
}


// returns scalar * a (s is scalar, a is vector)
v2d_t v2d_scale (v2d_t a, double scalar) {
	a.x *= scalar;
	a.y *= scalar;

	return a;
}


// returns a + b
v2d_t v2d_add (v2d_t a, v2d_t b) {
	a.x += b.x;
	a.y += b.y;

	return a;
}


// returns a - b
v2d_t v2d_sub (v2d_t a, v2d_t b) {
	a.x -= b.x;
	a.y -= b.y;

	return a;
}


// returns a dot b (|a|*|b|*cos (theta))
double v2d_dot (v2d_t a, v2d_t b) {
	return ((a.x * b.x) + (a.y * b.y));
}


// returns a cross b (sorta)
double v2d_cross (v2d_t a, v2d_t b) {
	return ((a.x * b.y) - (a.y * b.x));
}


// returns a vevtor rotated theta radians around the origin
v2d_t v2d_rot (v2d_t a, double theta) {
	v2d_t b;

	b.x = (cos (theta) * a.x) - (sin (theta) * a.y);
	b.y = (sin (theta) * a.x) + (cos (theta) * a.y);

	return b;
}


// returns the opposite vector
v2d_t v2d_neg (v2d_t a) {
	a.x = -a.x;
	a.y = -a.y;

	return a;
}


// returns a perpendicular vector
v2d_t v2d_normal (v2d_t a) {
	double t = a.x;
	a.x = a.y;
	a.y = -t;

	return a;
}


// returns the angle off the x-axis
double v2d_theta (v2d_t a) {
	return atan2 (a.y, a.x);
}


// returns the projection of a onto b
v2d_t v2d_project (v2d_t a, v2d_t b) {
	v2d_t p;
	double dot = (a.x * b.x) + (a.y * b.y); // dot product of a and b
	double bsq = (b.x * b.x) + (b.y * b.y); // |b|^2

	// i can't deal with this! bail!
	if (bsq == 0.0) return v2d_zero ();

	// so: dot = DOT (b) / (b.x * b.x + b.y * b.y)
	dot /= bsq;

	p.x = dot * b.x;
	p.y = dot * b.y;

	return p;
}


// returns the v2d that is located {percent} of the distance from a to b
v2d_t v2d_interpolate (v2d_t a, v2d_t b, double percent) {
	v2d_t ret;

	ret.x = a.x + ((b.x - a.x) * percent);
	ret.y = a.y + ((b.y - a.y) * percent);

	return ret;
}


#endif
