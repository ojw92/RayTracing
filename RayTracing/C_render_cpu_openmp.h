// C_render_cpu_openmp.h
#pragma once
#include "C_image.h"
#ifdef _OPENMP      // compile with or without OpenMP available
#include <omp.h>
#endif

// Parallel Programming
// OpenMP is used here to simplify multi-threading compared to how std::thread was used for manual threading in C_render_cpu_threads.h

inline void render_cpu_openmp(Image& img) {
    const int nx = img.width, ny = img.height;

#pragma omp parallel for schedule(dynamic, 4)   // split rows across threads, allowing threads to grab rows in chunks of 4 for better load balancing for uneven work
    for (int j = 0; j < ny; ++j) {
        int jj = ny - 1 - j;
        for (int i = 0; i < nx; ++i) {
            float r = float(i) / float(nx);
            float g = float(jj) / float(ny);
            float b = 0.2f;
            auto* p = img.pixel_ptr(i, j);
            p[0] = (uint8_t)(255.99f * r);
            p[1] = (uint8_t)(255.99f * g);
            p[2] = (uint8_t)(255.99f * b);
        }
    }
}
