#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:             // visible to anyone who uses this class
    ray() {}        // default constructor

    // Declare a constructor that sets origin and direction and uses the member initializer list ": orig(origin), dir(direction)"
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}      // point3 is alias of vec3

    // Define getter functions - "const ... const" returns a const reference (can¡¯t modify the internals via this return)
    const point3& origin() const { return orig; }   // starting point of ray
    const vec3& direction() const { return dir; }   // direction vector of ray

        // 'const point3&' means the function returns a reference to the internal member, but the reference is read-only and immutable
        // ray::origin() and ray::direction() both return an immutable reference to their members

    // Parametric ray equation: P(t) = origin + t * direction
    point3 at(double t) const {
        return orig + t * dir;
    
        // for positive t, you only get vector that goes in front of the origin -> half-line of a ray
    }

private:            // internal data that only the class itself can access
    point3 orig;    // stores 3D starting point of ray
    vec3 dir;       // stores direction vector

};

// caller is code that calls the function
// If the caller writes "const point3& o = r.origin();" o is now an alias for r.orig; it is not a copy (efficient) and is immutable
// If the caller writes "point3 o = r.origin();" a new point3 object with same values as r.orig is created; this copy belongs to the caller and can be modified freely
// To make a mutable copy, just add these to main():
    // ray r(point3(0,0,0), vec3(1,2,3));
    // point3 myOrigin = r.origin();        // mutable copy of origin via calling copy constructor
    // myOrigin.x() = 5.0;                  // modify copy freely assuming vec3/point3 has x(), y(), z() accessors
    // myOrigin.y() = 2.0;
    // myOrigin.z() = 1.0;
    // std::cout << "Ray origin: " << r.origin() << "\n";       // The original ray is unchanged
    // std::cout << "My modified copy: " << myOrigin << "\n";

    // Re-cap:
    // r.origin() returns a const reference to r.orig
    // point3 myOrigin = r.origin(); copies the data from r.orig into a brand-new point3
    // const point3& ref = r.origin();      // reference - read-only, no copy, but efficient
    // point3 copy = r.origin();            // copy - mutable, independent, but takes more memory/time
        // just like how tuple is immutable and efficient while an array is mutable and less efficient


#endif