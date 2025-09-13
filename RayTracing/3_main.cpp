#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <vector>       // vector, for writing pixel rgb to jpg

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/*
This program builds a rectangular view plane in front of a camera that traverses that plane, pixel by pixel, to observe the color of
each pixel when a ray is shot directly to pixel from the camera

The design of the viewport is done by building a rectangular “view plane” in front of the camera at z = -focal_length.
viewport_u (right) and viewport_v (down) span that rectangle.
You then compute the upper-left corner of that plane, then step in fixed deltas to hit each pixel center.
For each pixel, you cast a ray from the camera to the pixel center and ask ray_color for its color.

Output: write a PPM image to stdout. Progress goes to stderr so the image data stays clean.

Currently, the image is all black, but if a background, a sphere, normals, etc are added, an actual image can be observed
*/


color ray_color(const ray& r) {
    // return color(0, 0, 0);          // for now, fix it to color black (0,0,0)

    // Create a white~blue background as a simple sky with vertical gradient
    vec3 unit_direction = unit_vector(r.direction());   // 3D unit vector of the ray direction
    // Use y-value of unit_direction to decide how much blue vs white to blend
    auto a = 0.5 * (unit_direction.y() + 1.0);  // if y = -1 (top of viewport), a=0 (start of gradient); if y = 1 (bottom of viewport), a=1(end of gradient)
        // Another way to look at this:
            // y = -1 means ray is pointing downward -> t = 0 -> pure white
            // y = 1  means ray is pointing upward   -> t = 1 -> pure blue
        // A horizontal gradient is also present because a ray pointing to, say, the upper-left & -right corners have (-x, +y, -z) & (+x, +y, -z), respectively
            // However, y value isn't the same in both cases, since normalizing depends on (x,y,z) altogether
            // So across the screen, unit_driection.y() changes both vertically and horizontally - it "bends" towards corners a bit
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // lerp equation for white-to-blue gradient; a=1 for blue(end), a=0 for white(start)
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
    auto camera_center = point3(0, 0, 0);

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

    // Allocate RGB buffer (row-major order, top-to-bottom) for storing pixel values for jpg writing
    std::vector<unsigned char> image(image_width * image_height * 3);


    // Render

    // header for a PPM image file in the P3 (ASCII text) format that tells an image viewer how to parse the following pixel data
    // PPM readers expect exactly this format! To add more info for the human eyes without disrupting the format for the machine, add comments like so:
        // std::cout << "P3\n" << "# image_width: " << image_width << " image_height: " << image_height << "\n" << image_width << ' ' << image_height << "\n255\n"; 
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);    // coordinates of each (i,j)-th pixel's center
            auto ray_direction = pixel_center - camera_center;      // ray direction from camera to pixel center, but not unit vector for code simplicity
            ray r(camera_center, ray_direction);    // a ray class object is defined by origin of the ray (camera_center), direction of the ray (ray_direction) and a function at(t) to get a point along the ray (origin + t*direction) <- half ray if positive!

            // color pixel_color  ->  Declare variable 'pixel_color' of type 'color' (same as 'vec3') that holds RGB values for one pixel; this is set equal to ray_color(r), which computes the color for the ray going through this pixel, i.e., vec3/color
            color pixel_color = ray_color(r);       // based on the ray direction, find out what color the pixel is emitting (black if no object)
            // write_color(std::cout, pixel_color); // prints pixel RGB values on screen
            
            // Scale RGB values back to [0,255] from [0.0-1.0] to write into file
            int ir = static_cast<int>(255.999 * pixel_color.x());
            int ig = static_cast<int>(255.999 * pixel_color.y());
            int ib = static_cast<int>(255.999 * pixel_color.z());

            int index = (j * image_width + i) * 3;
            image[index + 0] = static_cast<unsigned char>(ir);
            image[index + 1] = static_cast<unsigned char>(ig);
            image[index + 2] = static_cast<unsigned char>(ib);
        }
    }


    // Write JPG file - saved at C:\Users\ohjin\OneDrive\문서\GitHub\RayTracing\out\build\x64-debug\RayTracing unless otherwise specified
    if (stbi_write_jpg("C:/Users/ohjin/OneDrive/문서/GitHub/RayTracing/RayTracing\\main_img.jpg", image_width, image_height, 3, image.data(), 90)) {
        std::cout << "Wrote main_img.jpg\n";}
    else {std::cout << "Failed to write image!\n";}

    std::clog << "\rDone.                 \n";
}