#ifndef RTWEEKEND_H     // if RTWEEKEND_H macro not defined, define it; future includes of this file will see RTWEEKEND_H is defined and skip its contents
#define RTWEEKEND_H

#include <cmath>        // math functions
//#include <cstdlib>    // outdated random
#include <random>       // modern random
#include <iostream>
#include <limits>       // type limits
#include <memory>       // smart pointers

/*
This is a common header file.
Program files will include rtweekend.h first, so all other header files (where the bulk of
our code will reside) can implicitly assume that rtweekend.h has already been included.
Header files still need to explicitly include any other necessary header files.
*/

// C++ Std Usings
    // Note: placing using declarations in a header pollutes all files that include it; generally avoided in production headers
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();    // the IEEE ¡°+inf¡± value for double
const double pi = 3.1415926535897932385;        // modern C++ option: inline constexpr double pi = std::numbers::pi; where std::numbers::pi is the standard predefined constant for pi
    // constexpr is "constant expression" that is computed at compile time, not runtime
    // normally, if const double pi is put in a header and included in many .cpp files, it would violate the One Definition Rule (ODR), but 'inline' tells the compiler that it's okay for that variable to appear in multiple files and treat them as a single variable
    // i.e., in headers, 'inline' is like saying 'allow this constant to be safely shared across all source files'

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1)
    // std::rand() returns a random integer between 0 and RAND_MAX, where RAND_MAX is an integer constant in cstdlib that represents max value that rand() can return
    // Lower quality, outdated random for simulations; uses the same seed across runs unless manually using std::srand(seed)
    // return std::rand() / (RAND_MAX + 1.0);

    // Alternate implementation of random using Mersenne Twister (std::mt19937)
    // static means the generator and distribution are intialized only once and reused every time the function is invoked -> efficient
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);   // true uniformity between 0.0 and 1.0
    static std::mt19937 generator;
    return distribution(generator);
        // seed the generator to get unique results each run using this  ->  static std::mt19937 generator(std::random_device{}());
}

inline double random_double(double min, double max) {
    // Returns a random real in [min, max)
    return min + (max - min) * random_double();
}

// Common Headers - includes project headers that depend on the utilities above
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif