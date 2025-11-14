#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

// #include <iostream>

// For writing image - these should be included in a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


/*
This program builds a rectangular view plane in front of a camera that traverses that plane, pixel by pixel, to observe the color of
each pixel when a ray is shot directly to pixel from the camera

The design of the viewport is done by building a rectangular ¡°view plane¡± in front of the camera at z = -focal_length.
viewport_u (right) and viewport_v (down) span that rectangle.
You then compute the upper-left corner of that plane, then step in fixed deltas to hit each pixel center.
For each pixel, you cast a ray from the camera to the pixel center and ask ray_color for its color.

Output: write a PPM image to stdout. Progress goes to stderr so the image data stays clean.

Currently, the image is all black, but if a background, a sphere, normals, etc are added, an actual image can be observed
*/


/*
// Implement math for intersecting sphere by placing a small sphere at -1 on the z-axis and then coloring red any pixel that intersects it
double hit_sphere(const point3& center, double radius, const ray& r) {    // bool: did the ray hit the sphere or not; double: where did it hit
    // Check whether ray r intersects a sphere
    vec3 oc = center - r.origin();      // (C-Q); vector from camera lens (origin) to sphere center
    
    // Refer to "Notes.docx" for the a, b, c values of the quadratic formula
    // Using b = -2d * (C-Q) = -2h, simplify the sphere-interaction code by factoring out the 4 under the sqrt
    auto a = r.direction().length_squared();        // d * d, same as before, just more concise
    auto h = dot(r.direction(), oc);                // d * (C-Q)
    auto c = oc.length_squared() - radius * radius; // same as before, just more concise
    auto discriminant = h * h - a * c;              // 4 factored out

    //auto a = dot(r.direction(), r.direction());     // d * d
    //auto b = -2.0 * dot(r.direction(), oc);         // -2d * (C-Q)
    //auto c = dot(oc, oc) - radius * radius;         // (C-Q) * (C-Q) - r^2
    //auto discriminant = b * b - 4 * a * c;          // value inside sqrt in quadratic formula; if +, 2 roots & ray hits twice; if 0, 1 root & ray hits once; if -, 0 roots & ray hits 0 times;

    if (discriminant < 0) {     // if 0 root (non-real solution), just return some negative value (to be used later)
        return -1.0;    // -1 as a sentinel value ("no hit"); sentinel value in C++ is a special, non-data value used to signal a specific condition, most commonly the termination of a loop or the end of a data sequence
    } else {
        return (h - std::sqrt(discriminant)) / a;
        //return (-b - std::sqrt(discriminant)) / (2.0 * a);  // if ray hits, return t, the parameter where the ray hits
    }
}
*/

// Sphere surface normals seem expensive to calculate because of the sqrt, but it's better to calculate just once up front
// Sphere normals can be made unit length by dividing by radius, avoiding the expensive sqrt
// Here, adopt the policy that normal vectors will be of unit length


int main() {
    // World
    hittable_list world;    // collection of 'hittable' objects; its 'hit' loops over all objects, keeping the closest valid hit

    // Adjustable camera test with two touching spheres
    //auto R = std::cos(pi / 4);
    //auto material_left = make_shared<lambertian>(color(0, 0, 1));
    //auto material_right = make_shared<lambertian>(color(1, 0, 0));
    //world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    //world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);    // 1.00 / 1.33; glass/dielectric material's total internal reflection illustrated via emulating world filled with water (index of refraction 1.33), with glass sphere as air bubble (index of refraction of 1.00) 
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);   // added fuzziness of 1.0

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));  // big sphere used as ground plane (radius=100, top is near y=0)
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));       // small sphere visible at z = -1.2
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    camera cam;

    // Image
    // Define the ideal aspect ratio and image width; image height can be calculated from these two values (done in camera.h)
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;    // set to 100; override default value of 10 in camera.h for slower rendering, but smoother image with less aliasing
    cam.max_depth = 50;

    cam.vfov = 20;                      // field of view
    cam.lookfrom = point3(-2, 2, 1);    // slightly off-center for alternate viewpoint
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.render(world);
}


// Some key notes to remember:
// ray equation: P(t) = (origin + t*direction)      <- if positive, it's a half ray