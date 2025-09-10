// C_render_cpu_baseline.h (to tidy up main.cpp)
#pragma once
#include "C_image.h"
#include "vec3.h"
#include "ray.h" 

// Provides a single-threaded renderer that loops over pixels to write a gradient for the graident image P3
// Simple and slow baseline against which to measure speedups (the "control") - same as 1_firstP3.cpp

inline void render_cpu_baseline(Image& img) {
    const int nx = img.width, ny = img.height;

    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2f;

            auto* p = img.pixel_ptr(i, ny - 1 - j);    // flip to write top->bottom
            p[0] = (uint8_t)(255.99f * r);
            p[1] = (uint8_t)(255.99f * g);
            p[2] = (uint8_t)(255.99f * b);
        }
    }
}
