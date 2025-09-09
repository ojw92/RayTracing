#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

/*
Defines color as an alias for vec3 and provides a write_color function that converts a floating-point RGB color (0.0–1.0)
into 8-bit values (0–255) and writes them to an output stream in text form, suitable for constructing PPM images
*/

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif