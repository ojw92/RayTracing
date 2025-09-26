#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
// #include "vec3.h"      // assume rtweekend.h inclusion

/*
This class describes one specific object - a sphere - that provides the info about the ray hitting it on the surface
*/

// Declare class sphere
class sphere : public hittable {    // 'sphere' class inherits from 'hittable' class ("things rays can hit"); done publicly, so outside code can treat 'sphere' as 'hittable'
public:     // everything after this is accessible and can be written or read from outside the class
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}   // ensure radius >= 0; std::fmax() returns maximum of two floats
        // sphere is constructor function that takes in 3D point for center and floating-point for radius
        // center(center) initializes member variable 'center' with constructor's parameter 'center'
        // radius(std::fmax(0, radius)) initializes the member variable 'radius' with either the provided radius or 0

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {  // input: ray being tested (read-only), min & max valid distance along the ray, output slot 'rec' to fill with hit info if there's intersection
                                                                                                // bool hit() const ensures that function won't modify 'sphere' itself; override invokes compiler check: confirms this function overrides a 'virtual' one from 'hittable'
        vec3 oc = center - r.origin();                  // vector from ray origin to sphere center
        auto a = r.direction().length_squared();        // sq length of ray direction vector
        auto h = dot(r.direction(), oc);                
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        // First check the nearer root (hit point closer to ray origin)
        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            // If it's out of range, check the farther root (hit point behind the sphere or on the far side)
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        // rec is a hit_record object defined in hittable.h
        rec.t = root;           // save valid intersection 'root' in the record
        rec.p = r.at(rec.t);    // intersection point
        //rec.normal = (rec.p - center) / radius;     // unit length outward normal
        vec3 outward_normal = (rec.p - center) / radius;    // unit length outward normal
        rec.set_face_normal(r, outward_normal);     // check the ray & outward normal for their alignment

        return true;
    }

private:            // data below can only accessed inside class
    point3 center;
    double radius;
};

#endif