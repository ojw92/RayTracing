#ifndef HITTABLE_H		// if HITTABLE_H is not defined yet, define it and include the file; otherwise, skip it; prevents the file from being included multiple times in the same translation unit
#define HITTABLE_H

#include "ray.h"

/*
This class answers the question: does a ray hit the object, and if so, where and how?
*/

// Class for information about ray-object intersection, or a "hit"
class hit_record {
public:				// members listed under public are accessible from anywhere, not just inside the class
	point3 p;		// 3D coordinate of the hit point (point3 is type alias for vec3)
	vec3 normal;	// surface normal vector at that point (for shading)
	double t;		// ray parameter t at the hit (r.at(t) = p); tells how far along the ray the intersection has occurred; floating-point (64-bit)
};

// Class for objects that a ray can hit
class hittable {
public:
	// Ensure that if a dervied object (sphere, plane, etc) is deleted though a pointer to "hittable", the derived destructor runs, as well
	// virtual makes the function polymorphic - tells the compiler this function can be overridden in derived classes
	virtual ~hittable() = default;	// a destructor ~ for the class cleans up when an object goes out of scope or is deleted; =default means generate default implementation 

	// Define a virtual function that returns a boolean;
	// putting const at the end of a member function (hit() in this case) does not modify the object's data members; only reading properties, not changing them
	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;	// =0 makes it a pure virtual function
};

#endif

// Read up on virtual function