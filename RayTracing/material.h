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

class lambertian : public material {
public:
	lambertian(const color& albedo) : albedo(albedo) {}	
		// const color& albedo: take albedo by const reference (no copy, not mutated)
		// albedo(albedo): initializes the data member albedo with the parameter albedo

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)	// scatter() is a method that decides how an incoming ray bounces
			// color& attenuation: output parameter; multiply the ray¡¯s color by this (the material¡¯s albedo)
			// ray& scattered: output parameter; the new scattered ray to trace next
		const override {	// const: this method does not modify the lambertian object itself; override: tells the compiler this function overrides material::scatter (checks the signature)
		// Returns true if the material produces a scattered ray, false if absorbed
		auto scatter_direction = rec.normal + random_unit_vector();	// the direction we'll shoot the scattered ray
			// adding the random_unit_vector() to the normal ¡æ picks a random direction in the hemisphere around the normal (Lambertian diffuse)
			// if the random unit vector is exact opposite of the normal vector, sum will be 0, resulting in zero scatter direction vector; vec3::near_zero() will handle this to return true if vector is very close to 0 in all dimensions
		
		// Catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;
		
		scattered = ray(rec.p, scatter_direction);	// sets the output ray, which has origin at the hit point and direction as the sampled diffuse direction
		attenuation = albedo;	// set the output color multiplier as albedo, the material's reflectance color - how much of incoming light is kept per channel; pure red diffuse would be (1, 0, 0), and gray as (0.5, 0.5, 0.5)
		return true;
	}

private:	// only the class¡¯s own methods can access these members
	color albedo;
};

class metal : public material {
public:
	metal(const color& albedo) : albedo(albedo) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)	// computes how an incoming ray bounces
		const override {
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

#endif