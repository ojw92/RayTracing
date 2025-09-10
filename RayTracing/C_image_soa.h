// C_image_soa.h
#pragma once
#include <cstdint>      // fixed-width integer types (uint8_t = 1 byte, 0–255)
#include <vector>
#include <string>
#include <fstream>

// Experimenting with cache-friendly and SIMD-friendly designs
// Use for vectorization and cache use during heavy math such as per-pixel path sampling (memory-architecture optimization)

// Memory-Architecture Optimization
// AoS is good for I/O; SoA is good for vectorization/SIMD (Single Instruction, Multiple Data)
    // SIMD - parallel computing technique where one instruction is executed on multiple data points simultaneously, increasing throughput and efficiency for tasks like multimedia processing, graphics, and scientific computations
// Structure of Arrays (SoA): instead of storing [R,G,B] together per pixel, each channel is its own continuous array
// Array of Structures (AoS): each pixel is stored as [R,G,B]

struct ImageSoA {
    int width, height;              
    std::vector<uint8_t> R, G, B;   // This separates channel arrays (SoA) rather than interleaved RGB (AoS) to help vectorization and cache efficiency

    // Initialize w & h and allocate space in each channel array for all pixels with index i = y*width + x
    ImageSoA(int w, int h) : width(w), height(h), R(w* h), G(w* h), B(w* h) {}

    // Define idx() to convert 2D pixel coordinates to 1D index into each channel array (refer to C_image.h)
    inline int idx(int x, int y) const { return y * width + x; }

    // Write image to PPM format (P6)
    void write_ppm(const std::string& path) const {
        std::ofstream out(path, std::ios::binary);      // open file stream in binary
        out << "P6\n" << width << " " << height << "\n255\n";       // P6 = binary RGB format
        std::vector<uint8_t> packed(width * height * 3);    // creates temporary buffer to hold RGB data in interleaved format because PPM expects AoS not SoA
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                // Calculate index i and store in R, G, and B arrays
                int i = idx(x, y);
                packed[3 * i + 0] = R[i];
                packed[3 * i + 1] = G[i];
                packed[3 * i + 2] = B[i];
            }
        out.write(reinterpret_cast<const char*>(packed.data()), packed.size());     // reinterpret_cast tells the compiler to treat the raw bytes as characters
    }
};
