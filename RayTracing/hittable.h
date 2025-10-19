#ifndef HITTABLE_H		// if HITTABLE_H is not defined yet, define it and include the file; otherwise, skip it; prevents the file from being included multiple times in the same translation unit
#define HITTABLE_H

// #include "ray.h"      // assume rtweekend.h inclusion

class material;

// Class for information about ray-object intersection, or a "hit": does a ray hit the object, and if so, where and how?
class hit_record {
public:					// members listed under public are accessible from anywhere, not just inside the class
	point3 p;			// 3D coordinate of the hit point (point3 is type alias for vec3)
	vec3 normal;		// surface normal vector at that point (for shading)
	shared_ptr<material> mat;	// since we're just specifying a pointer to the class, the compiler doesn't need to know the details of the class
	double t;			// ray parameter t at the hit (r.at(t) = p); tells how far along the ray the intersection has occurred; floating-point (64-bit)
	bool front_face;	// false if dot(ray_direction, outward_normal) > 0.0, i.e., ray is inside the sphere; true if dot(ray_direction, outward_normal) <= 0.0, i.e., ray is outside the sphere; dot product can be 0 if the ray is tangent to sphere, i.e., outside the sphere and/or inside the sphere; so = sign can be used for either case

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		// Sets the hit record normal vector
		// Note: the parameter 'outward_normal' is assumed to have unit length
			// More notes: 'const ray& r' means that 'r' is a reference for an existing object of type 'ray', and this referred-to object cannot be changed through this parameter inside the function ('const'); not a pointer, though, since there's no * or ->
			// In other words, we are using 'r' as a way of referring to this object without the risk of changing the object.
			// For example, in Python terms:
				// x = 1 
				// void some_function(const int& x) 
				// ... 
				// some_function(y) 
			// where y is basically x with a value of 1, but y does not alter the value of x directly
			// This is beneficial because this avoids copying a potentially large object and guarantees that you won't(can't) change it

		front_face = dot(r.direction(), outward_normal) < 0;	// true if ray points against outward normal (ray is outside object), false if ray aligns with outward normal (ray is inside object)
		normal = front_face ? outward_normal : -outward_normal;	// if true, outward_normal; else, -outward_normal
	}
};

// Class for objects that a ray can hit
class hittable {
public:
	// Ensure that if a dervied object (sphere, plane, etc) is deleted though a pointer to "hittable", the derived destructor runs, as well
	// virtual makes the function polymorphic - tells the compiler this function can be overridden in derived classes
	virtual ~hittable() = default;	// a destructor ~ for the class cleans up when an object goes out of scope or is deleted; =default means generate default implementation 

	// Define a virtual function that returns a boolean;
	// putting const at the end of a member function (hit() in this case) does not modify the object's data members; only reading properties, not changing them
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;	// =0 makes it a pure virtual function
};

#endif

// Read up on virtual function