// C_image.h
#pragma once
#include <cstdint>      // fixed-width integer types (uint8_t)
#include <vector>       // vector
#include <string>       // strings
#include <fstream>      // file I/O

// Provides container for rendered pixels that renderers can write into
// Also provides image export to see the results visually

struct Image {
    int width, height;
    std::vector<uint8_t> pixels;    // pixels is a flat vector of size = width * height * 3 for RGB buffer

    Image(int w, int h) : width(w), height(h), pixels(w* h * 3, 0) {}   // initialize dim and allocates w*h*3 bytes (3 channels) as 0's

    inline uint8_t* pixel_ptr(int x, int y) {   // return pointer to first byte of pixel (x,y)
        return &pixels[3 * (y * width + x)];    // indexing for 2D image in 1D array: (2,1) = row 1, col 2 = 1*4 + 2 = 6; this does row * width + col, but 3 times for RGB
    }

    // Simple PPM writer (portable, no deps)
    void write_ppm(const std::string& path) const {
        std::ofstream out(path, std::ios::binary);
        out << "P6\n" << width << " " << height << "\n255\n";
        out.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    }
};
