// C_main.cu
#include <iostream>
#include <vector>
#include <cstdint>
#include <cuda_runtime.h>
#include "C_image.h"     

#include "C_timer.h"
#include "C_render_cpu_baseline.h"
#include "C_render_cpu_threads.h"
// #include "C_render_cpu_openmp.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION      // shouldn't declare if 1_firstP3.cpp were part of RayTracingCUDA.exe under CMakeLists.txt
#include "stb_image_write.h"                // Write jpg


// CUDA kernel for computing gradient on the GPU, saving the Image result as PPM
// Allocates pixel buffer in unified memory via cudaMallocManaged
// Launches thousands of threads (1/pixel)
// Practicing GPU acceleration and parallelism via thread heirarchy (grid/block/threadIdx), memory management and synchronization via cudaDeviceSynchronize()
    // "embarrassingly parallel" problem can be divided into completely independent sub-problems that can be processed simultaneously with minimal or no communication or dependency between them.
    // "embarrassingly" refers to how easy the parallelization is, as the task is so straightforward that it's almost "embarrassing" to need a complex parallel setup for it.

// Define a device kernel that runs on the GPU and launches many threads
__global__ void gradient_kernel(uint8_t* pixels, int width, int height) {
    // Compute the pixel this GPU thread is responsible for from its block and thread indices
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= width || y >= height) return;      // ensure edges of the grid do not overshoot

    // Flip Y like CPU version (top->bottom rendering)
    int jj = (height - 1 - y);

    float r = float(x) / float(width);
    float g = float(jj) / float(height);
    float b = 0.2f;
    
    // Gradient pixel stored as AoS
    int idx = 3 * (y * width + x);
    pixels[idx + 0] = (uint8_t)(255.99f * r);
    pixels[idx + 1] = (uint8_t)(255.99f * g);
    pixels[idx + 2] = (uint8_t)(255.99f * b);
}

int main() {
    const int W = 7680, H = 4320;
    const size_t bytes = W * H * 3;

    // CPU Baseline
    Image img_cpu_base(W, H);   // Image is a CPU-side image class and expects standard CPU memory for saving .ppm and .jpg
    Timer timer_cpu_base;
    timer_cpu_base.tic();

    render_cpu_baseline(img_cpu_base);   // Your CPU single-threaded function
    double cpu_base_time = timer_cpu_base.toc_ms();

    img_cpu_base.write_ppm("C:/Users/ohjin/OneDrive/문서/GitHub/RayTracing/RayTracing/cpu_baseline.ppm");
    stbi_write_jpg("C:/Users/ohjin/OneDrive/문서/GitHub/RayTracing/RayTracing/cpu_baseline.jpg", W, H, 3, img_cpu_base.pixels.data(), 90);
    std::cout << "CPU baseline execution time: " << cpu_base_time << " ms\n";


    // CPU multithreaded
    Image img_cpu_threads(W, H);
    Timer timer_threads;
    timer_threads.tic();

    render_cpu_threads(img_cpu_threads);   // Your CPU multithreaded function
    double cpu_threads_time = timer_threads.toc_ms();

    img_cpu_threads.write_ppm("C:/Users/ohjin/OneDrive/문서/GitHub/RayTracing/RayTracing/cpu_threads.ppm");
    stbi_write_jpg("C:/Users/ohjin/OneDrive/문서/GitHub/RayTracing/RayTracing/cpu_threads.jpg", W, H, 3, img_cpu_threads.pixels.data(), 90);
    std::cout << "CPU multi-threaded time: " << cpu_threads_time << " ms\n";


    // CUDA GPU
    // Unified memory for simplicity
    uint8_t* d_pixels = nullptr;
    cudaMallocManaged(&d_pixels, bytes);        // allocate unified memory accessible by both CPU and GPU

    // Set a 2D block size and compute enough blocks to cover the image (ceiling division)
    dim3 block(16, 16);
    dim3 grid((W + block.x - 1) / block.x, (H + block.y - 1) / block.y);

    // Begin timing
    Timer timer_cuda;
    timer_cuda.tic();

    // Launch the kernel and wait for it to finish
    gradient_kernel << <grid, block >> > (d_pixels, W, H);      // launch kernel
    cudaDeviceSynchronize();                                    // wait for kernel to sync

    // End timing
    double cuda_time = timer_cuda.toc_ms();                     // measure elapsed time

    Image img_cuda(W, H);   // copy the unified memory into the CPU Image object and save it
    std::memcpy(img_cuda.pixels.data(), d_pixels, bytes);
    img_cuda.write_ppm("C:/Users/ohjin/OneDrive/문서/GitHub/RayTracing/RayTracing/cuda_output.ppm");
    stbi_write_jpg("C:/Users/ohjin/OneDrive/문서/GitHub/RayTracing/RayTracing/cuda_output.jpg", W, H, 3, d_pixels, 90);

    cudaFree(d_pixels);     // free GPU memory

    std::cout << "CUDA GPU-accelerated execution time: " << cuda_time << " ms\n";


    // Performance comparison
    std::cout << "GPU speedup vs CPU baseline : " << (cpu_base_time / cuda_time) << "x\n";
    std::cout << "GPU speedup vs CPU threads : " << (cpu_threads_time / cuda_time) << "x\n";
    std::cout << "Multithreading speedup vs baseline : " << (cpu_base_time / cpu_threads_time) << "x\n";


    return 0;
}

// Note: 1200 x 600 = 720,000 pixels; output image is about 33 KB - CPU is bound to be faster than multi-threading or GPU-acceleration
// HD (1920 x 1080 = 2.08M) or 4K (3840 x 2160 = 8.3M) images is where multi-threading & GPU-acceleration would shine
// Benchmark Results (1 trial each):
    // 1200 x 600 or 720,000 pixel image: CPU baseline: 2.842 ms; CPU multi-thread: 7.6332 ms; GPU CUDA: 3.8374 ms
        // GPU speedup vs CPU baseline : 0.7406x; GPU speedup vs CPU threads : 1.9892x; Multithreading speedup vs baseline : 0.37232x
    // 1920 x 1080 or 2,073,600 pixel image: CPU baseline: 7.7165 ms; CPU multi-thread: 9.6203 ms; GPU CUDA: 2.9483 ms
        // GPU speedup vs CPU baseline : 2.61727x; GPU speedup vs CPU threads : 3.263x; Multithreading speedup vs baseline : 0.802106x
    // 3840 x 2160 or 8,294,400 pixel image: CPU baseline: 31.1835 ms; CPU multi-thread: 13.068 ms; GPU CUDA: 3.7394 ms
        // GPU speedup vs CPU baseline : 8.33917x; GPU speedup vs CPU threads : 3.49468x; Multithreading speedup vs baseline : 2.38625x
    // 7680 × 4320 or 33,177,600 pixel image: CPU baseline: 123.91 ms; CPU multi-thread: 34.9096 ms; GPU CUDA: 3.4855 ms
        // GPU speedup vs CPU baseline : 35.5502x; GPU speedup vs CPU threads : 10.0157x; Multithreading speedup vs baseline : 3.54946x
// Truly shines for 4K and beyond