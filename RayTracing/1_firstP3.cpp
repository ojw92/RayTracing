// RayTracing.cpp : Defines the entry point for the application.
//
// std = standard library namespace; std::cout = character output stream object; std::ofstream = output file stream class

#include "RayTracing.h"
#include <iostream>		// for input/output like std::cout
#include <fstream>		// for writing to files (ofstream) or reading from files
#include <vector>		// standard library header file that defines the 'std::vector' container, which is dynamic array; can change size automatically (resize, push_back, etc.) and keep track of their own memory and thus are safer & more flexible

// Writing jpg - https://psgraphics.blogspot.com/2015/06/a-small-image-io-library-stbimageh.html
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;	// kinda like allowing collections.deque() --> deque(); std::cout -> cout

int main()
{
	// Pixels are written out in rows with pixels left to right
	// The rows are written out from top to bottom
	int nx = 200;
	int ny = 100;

	// a << b = stream insertion operator; send b into the stream a;	std::cout << "Hello"  prints "Hello" to the console

	std::vector<unsigned char> image(nx * ny * 3);	// dynamic array of elements of type 'unsigned char', which is a type that can hold values in 0~255; size nx * ny * 3 

	ofstream outfile("firstP3.ppm");					// open output file in text mode
	outfile << "P3\n" << nx << " " << ny << "\n255\n";	// write PPM header (P3 format); P3 indicates ASCII PPM file; nx ny are w & h; 255 max color value of each channel; " " inserts a space
	
	int idx = 0;	// idx for image array
	for (int j = ny - 1; j >= 0; j--)	{				// loop over rows from top to bottom (PPM expects top to bottom)
		for (int i = 0; i < nx; i++) {					// loop over cols from left to right
			// Compute color gradient
			float r = float(i) / float(nx);				// red increases left to right
			float g = float(j) / float(ny);				// green increases bottom to top
			float b = 0.2;								// blue constant at 20% brightness

			// Convert floats into integer RGB values (0-255)
			int ir = int(255.99 * r);
			int ig = int(255.99 * g);
			int ib = int(255.99 * b);

			image[idx++] = (unsigned char)(255.99 * r);
			image[idx++] = (unsigned char)(255.99 * g);
			image[idx++] = (unsigned char)(255.99 * b);

			// std::cout << ir << " " << ig << " " << ib << "\n";	// print pixels
			// Write pixel values into file
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}

	// Files saved at C:\Users\ohjin\OneDrive\문서\Cpp\RayTracing\out\build\x64-debug\RayTracing
	if (stbi_write_jpg("firstP3.jpg", nx, ny, 3, image.data(), 90)) {
		std::cout << "Wrote firstP3.jpg\n";
	}
	else {
		std::cout << "Failed to write image!\n";
	}

	outfile.close();		// close file to save data to disk

	cout << "Image written to firstP3.ppm" << endl;
	return 0;
}
