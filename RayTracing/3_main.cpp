#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

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


color ray_color(const ray& r) {
    return color(0, 0, 0);
}

int main() {

    // Image

    // Define the ideal aspect ratio and image width - image height can be calculated from these two values
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;       // image height must be at least 1, so return 1 if smaller, else image_height

    // Camera
        // auto tells the compiler to deduce the type of a variable from its initializer and allcate the proper amount of memory
    auto focal_length = 1.0;        // distance from camera center to image plane along the -Z (into the image)
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
        // viewport width calculated by using the "aspect ratio", which may be different from aspect_ratio since image_height might have been safe clamped to a value of 1 instead of negative or fractional value
        // value in aspect_ratio is the ideal value, not the actual ratio between image_width and image_height
    auto camera_center = point3(0, 0, 0)

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);       // horizontal axis, increasing to the right
    auto viewport_v = vec3(0, -viewport_height, 0);     // vertical axis, increasing to the bottom, since image scan is done from top to bottom

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel via traversing the 3D space
    auto viewport_upper_left =
        camera_center               // start from the camera
        - vec3(0, 0, focal_length)  // move from camera to the center of the view plane into screen
        - viewport_u / 2            // shift left by half viewport width to reach the left edge of the viewport
        - viewport_v / 2;           // shift up (since v is down) by half viewport height to reach the top edge of the viewport
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);     // from upper left corner, move to the center of the top-left pixel (0,0)

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);    // coordinates of each (i,j)-th pixel's center
            auto ray_direction = pixel_center - camera_center;                              // ray direction from camera to pixel center
            ray r(camera_center, ray_direction);    // a ray class object is defined by origin of the ray (camera_center), direction of the ray (ray_direction) and a function at(t) to get a point along the ray (origin + t*direction) <- half ray if positive!

            color pixel_color = ray_color(r);       // based on the ray direction, find out what color the pixel is emitting (black if no object)
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}