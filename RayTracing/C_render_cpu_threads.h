// C_render_cpu_threads.h
#pragma once
#include <thread>       // std::thread for creating worker threads
#include <atomic>       // std::atomic for coordinating work between threads without locks
#include <vector>
#include "C_image.h"    // to write pixels in Image containers

// Parallel Programming
// Achieves parallel pixel rendering via manual multi-threading (std::thread) with speedup proportional to core count
// Allows benchmarking of multi-core scaling and scaling rendering across CPU cores
// Demonstrates dynamic load balancing via an atomic work queue instead of static row splitting

// Define inline function with Image & num_threads (default of 1 per core) as input to render gradients into img using multiple threads
inline void render_cpu_threads(Image& img, int num_threads = std::thread::hardware_concurrency()) { 
    const int nx = img.width, ny = img.height;      // create local copies of nx, ny

    // Atomic counter starting at 0; each worker thread will "fetch & increment" this counter to claim the next row of pixels to render
    std::atomic<int> next_row{ 0 };     // 'atomic' makes this thread-safe without explicit locks or mutexes; mutual exclusion (mutex) is a program object that prevents multiple threads from accessing the same shared resource simultaneously ("single-occupancy restroom key");

    auto worker = [&]() {   // defines lambda function; [&] means "capture by reference", meaning lambda can access img, nx, ny and next_row directly
        int j;              // row index for this thread

        while ((j = next_row.fetch_add(1, std::memory_order_relaxed)) < ny) {   // fetch_add(1) atomically increments next_row and returns its previous value; if returned row index is still less than ny (image height), the thread renders that row; memory_order_relaxed tells the compiler that it only needs atomicity, not ordering (this is fine since rows are independent)
            int jj = ny - 1 - j;        // write scanlines top to bottom (memory naturally runs bottom to top); this flip (ny-1-j) ensures the image is not upside down
            for (int i = 0; i < nx; ++i) {
                float r = float(i) / float(nx);
                float g = float(jj) / float(ny);
                float b = 0.2f;
                auto* p = img.pixel_ptr(i, j);  // finds a pointer to the start of pixel (i,j) in the Image buffer
                p[0] = (uint8_t)(255.99f * r);
                p[1] = (uint8_t)(255.99f * g);
                p[2] = (uint8_t)(255.99f * b);
            }
        }
        };

    std::vector<std::thread> pool;      // creates a container to hold all worker threads
    pool.reserve(num_threads);          // reserves space to avoid reallocations
    for (int t = 0; t < num_threads; ++t) pool.emplace_back(worker);    // launches num_threads copies of the worker lambda
    for (auto& th : pool) th.join();    // waits for all threads to finish and ensures the image is fully rendered before the function returns
}
