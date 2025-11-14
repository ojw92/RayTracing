#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

#include <vector>       // vector, for writing pixel rgb to jpg
#include "stb_image_write.h"    // macro & definition should be in .cpp file

class camera {
public:
    double aspect_ratio      = 1.0;     // ratio of image width over height
    int    image_width       = 100;     // rendered image width in pixel count
    int    samples_per_pixel = 10;      // count of random samples for each pixel; default value and can be overridden in 3_main.cpp
    int    max_depth         = 10;      // max number of ray bounces into scene  

    double vfov = 90;                   // vertical view angle (field of view)
    point3 lookfrom = point3(0, 0, 0);  // point camera is looking from
    point3 lookat = point3(0, 0, -1);   // point camera is looking at
    vec3   vup = vec3(0, 1, 0);         // camera-relative "up" direction


    // Render
    void render(const hittable& world) {
        initialize();

        // header for a PPM image file in the P3 (ASCII text) format that tells an image viewer how to parse the following pixel data
        // PPM readers expect exactly this format! To add more info for the human eyes without disrupting the format for the machine, add comments like so:
            // std::cout << "P3\n" << "# image_width: " << image_width << " image_height: " << image_height << "\n" << image_width << ' ' << image_height << "\n255\n"; 
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        // Allocate RGB buffer (row-major order, top-to-bottom) for storing pixel values for jpg writing
        std::vector<unsigned char> image(image_width * image_height * 3);

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                /*
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);    // coordinates of each (i,j)-th pixel's center
                auto ray_direction = pixel_center - center;      // ray direction from camera to pixel center, but not unit vector for code simplicity
                ray r(center, ray_direction);    // a ray class object is defined by origin of the ray (camera_center), direction of the ray (ray_direction) and a function at(t) to get a point along the ray (origin + t*direction) <- half ray if positive!

                // color pixel_color  ->  Declare variable 'pixel_color' of type 'color' (same as 'vec3') that holds RGB values for one pixel; this is set equal to ray_color(r), which computes the color for the ray going through this pixel, i.e., vec3/color
                color pixel_color = ray_color(r, world);       // based on the ray direction, find out what color the pixel is emitting (black if no object)
                // write_color(std::cout, pixel_color); // prints pixel RGB values on screen
                */
                color pixel_color(0, 0, 0);
                // Shoot n=sample rays per pixel, accumulating the total color across all samples to 'pixel_color'
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                pixel_color = pixel_samples_scale * pixel_color;    // averaged accumulated color values from samples of the pixel; scale pixel_color by pixel_sample_scale
                write_color(std::cout, pixel_color);                // prints out averaged color

                // Scale RGB values back to [0,255] from [0.0-1.0] to write into file
                int ir = static_cast<int>(255.999 * linear_to_gamma(pixel_color.x()));  // need gamma correction for each RGB value being written
                int ig = static_cast<int>(255.999 * linear_to_gamma(pixel_color.y()));
                int ib = static_cast<int>(255.999 * linear_to_gamma(pixel_color.z()));

                int index = (j * image_width + i) * 3;
                image[index + 0] = static_cast<unsigned char>(ir);
                image[index + 1] = static_cast<unsigned char>(ig);
                image[index + 2] = static_cast<unsigned char>(ib);
            }
        }

        // Write JPG file - saved at C:\Users\ohjin\OneDrive\문서\GitHub\RayTracing\out\build\x64-debug\RayTracing unless otherwise specified
        if (stbi_write_jpg("C:/Users/ohjin/OneDrive/문서/GitHub/RayTracing/RayTracing\\main_img.jpg", image_width, image_height, 3, image.data(), 90)) {
            std::cout << "Wrote main_img.jpg\n";
        }
        else { std::cout << "Failed to write image!\n"; }

        std::clog << "\rDone.                 \n";
    }

private:
    // Declare members (fields of every camera object) - variables belonging to class object
    int    image_height;         // rendered image height
    double pixel_samples_scale;  // color scale factor for a sum of pixel samples
    point3 center;               // camera center
    point3 pixel00_loc;          // location of pixel 0, 0
    vec3   pixel_delta_u;        // offset to pixel to the right
    vec3   pixel_delta_v;        // offset to pixel below
    vec3   u, v, w;              // camera frame basis vectors

    void initialize() {
        // Calculate the image height, and ensure that it's at least 1
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;       // image height must be at least 1, so return 1 if smaller, else image_height

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine camera's viewport dimensions.
            // auto tells the compiler to deduce the type of a variable from its initializer and allcate the proper amount of memory
        auto focal_length = (lookfrom - lookat).length();        // distance from camera center to image plane along the -Z (into the image)
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * (double(image_width) / image_height);
            // viewport width calculated by using the "aspect ratio", which may be different from aspect_ratio since image_height might have been safe clamped to a value of 1 instead of negative or fractional value
            // value in aspect_ratio is the ideal value, not the actual ratio between image_width and image_height

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        vec3 viewport_u = viewport_width * u;    // vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // vector down viewport vertical edge
            //auto viewport_u = vec3(viewport_width, 0, 0);       // horizontal axis, increasing to the right
            //auto viewport_v = vec3(0, -viewport_height, 0);     // vertical axis, increasing to the bottom, since image scan is done from top to bottom

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel via traversing the 3D space
        auto viewport_upper_left = center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
        /*
        auto viewport_upper_left =
            center                      // start from the camera
            - vec3(0, 0, focal_length)  // move from camera to the center of the view plane into screen
            - viewport_u / 2            // shift left by half viewport width to reach the left edge of the viewport
            - viewport_v / 2;           // shift up (since v is down) by half viewport height to reach the top edge of the viewport
        */
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);     // from upper left corner, move to the center of the top-left pixel (0,0)
    }

    ray get_ray(int i, int j) const {   // generates different samples for each pixel
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        // Construct 3D position of sampled point inside pixel (i, j)
        auto pixel_sample = pixel00_loc             // top left corner (0,0)
            + ((i + offset.x()) * pixel_delta_u)    // how far to go right * 3D offset to move right by one pixel
            + ((j + offset.y()) * pixel_delta_v);   // how far to go down  * 3D offset to move down  by one pixel

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);      // return a 'ray' object that starts at the camera and goes toward the pixel sample
    }

    // Generate a random sample point within the unit square centered at the origin, with random 2D offset of [-0.5, +0.5]
    // By "jittering the ray" within the pixel instead of at the center, we can take average of multiple slightly-different samples per pixel, achieving anti-aliasing by blurring out sharp sampling edges
    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    // Generating sample point within non-square pixel
    vec3 sample_disk(double radius) const {
        // Returns a random point in hte unit (radius 0.5) disk centered at the origin
        //return radius * random_in_unit_disk();
    }

    // Key improvement: ray_color no longer knows/cares whether the scene has 1 sphere or 10,000 mixed shapes - it just queries 'world'
    // 9.2 Depth Limiting: ray_color() is recursive - set a max_depth to stop recursion when the ray does not hit anything
    color ray_color(const ray& r, int depth, const hittable& world) const {    // update to account for 'world', which is any object that implements the hittable interface; in practice, a hittable_list (a vector of objects) is passed
        /*
        // 6.1 - Rendering surface normals on a sphere
        auto t = hit_sphere(point3(0, 0, -1), 0.5, r);       // output of hit_sphere function saved to t (-1 or value computed via quadratic formula)
        if (t > 0.0) {
            // N = unit surface normal (points from sphere center to where the ray hits on the surface)
            vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));     // r.at(t) = point of intersection between ray and sphere; sphere centered at vec3(0,0,-1)
            return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);// N ranges [-1,1], so +1 and *0.5 scales range to [0,1]; color() outputs the color of it, converting from floating point [0.0-1.0] to 8-bit [0-255]
        }
        */
        // 9.2 If the ray bounce limit is exceeded, no more light is gathered
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {     // ask the entire scene to find the closest intersection in the range [tmin, tmax] and fill rec (point, normal, t, etc.)
            // Material-dependent scattered reflectance
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))   // call the material-specific scatter (polymorphism); true means we have scattered ray and attenuation color
                return attenuation * ray_color(scattered, depth - 1, world);    // this is the returned color
            return color(0, 0, 0);      // the if-statement returned false -> no scattered ray -> return black
            
            
            vec3 direction = rec.normal + random_unit_vector();     // implement true Lambertian reflection     // replicate diffuse material via random_on_hemisphere(rec.normal);
            // If a ray bounces off of a material and keeps 100% of its color, then we say that the material is white; for 0% it's black
            return 0.5 * ray_color(ray(rec.p, direction), depth-1, world);   // 50% color is returned from the bounce for gray rendering
            //return 0.5 * (rec.normal + color(1, 1, 1));   // visualize the normal if there's a hit; rec.normal ranges in [-1,1], so 0.5*(normal + (1,1,1)) remaps to [0,1] for display
        }
        // If the ray hits a sphere at center of viewpoint (0,0,-1) with radius 0.5, it returns red RGB value of (1,0,0)
        //if (hit_sphere(point3(0, 0, -1), 0.5, r))
        //    return color(1, 0, 0);  // it'll be a solid red circle; changing sphere center to z=+1 gets exactly the same picture because this solution doesn't distinguish between objects in front of the camera and objects behind the camera

        // Create a white~blue background as a simple sky with vertical gradient
        vec3 unit_direction = unit_vector(r.direction());   // 3D unit vector of the ray direction
        // Use y-value of unit_direction to decide how much blue vs white to blend
        auto a = 0.5 * (unit_direction.y() + 1.0);  // if y = -1 (top of viewport), a=0 (start of gradient); if y = 1 (bottom of viewport), a=1(end of gradient)
        // Another way to look at this:
            // y = -1 means ray is pointing downward -> a = 0 -> pure white
            // y = 1  means ray is pointing upward   -> a = 1 -> pure blue
        // A horizontal gradient is also present because a ray pointing to, say, the upper-left & -right corners have (-x, +y, -z) & (+x, +y, -z), respectively
            // However, y value isn't the same in both cases, since normalizing depends on (x,y,z) altogether
            // So across the screen, unit_driection.y() changes both vertically and horizontally - it "bends" towards corners a bit
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // lerp equation for white-to-blue gradient; a=1 for blue(end), a=0 for white(start)
        // return color(0, 0, 0);          // fix it to color black (0,0,0)
    }
};

#endif