#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
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

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));      // small sphere visible at z=-1
    world.add(make_shared<sphere>(point3(0, -100.5, 01), 100)); // big sphere used as ground plane (radius=100, top is near y=0)

    // Camera
    camera cam;

    // Image
    // Define the ideal aspect ratio and image width; image height can be calculated from these two values (done in camera.h)
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.render(world);
}


// Some key notes to remember:
// ray equation: P(t) = (origin + t*direction)      <- if positive, it's a half ray