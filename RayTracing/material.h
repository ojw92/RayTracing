#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
public:
	virtual ~material() = default;	// ~material() is a destructor - the function called when the object is destroyed; default destructor is created
		// It is a virtual destructor, which is mandatory when using polymorphism; ensures that when you destroy a derived class (like metal) via a material* pointer, the right destructor is called, preventing memory / resource leaks

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered	
			// ray& : read-only reference to the incoming ray
			// hit_record& : read-only intersection info (hit point p, surface normal, t, front/back face, etc)
			// color& attenuation: output parameter; the RGB multiplier applied to the ray¡¯s energy (e.g., albedo); non-const reference because scatter will write into it
			// ray& scattered: output parameter; the newly generated scattered ray (reflection/refraction/diffuse bounce)
	) const {	// this member function promises not to modify the material object itself
		return false;
	}
};

#endif