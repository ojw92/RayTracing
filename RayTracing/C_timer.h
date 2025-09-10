// C_timer.h
#pragma once
#include <chrono>       // high-resolution clock utilities

// Used for timing code operation

struct Timer {
    std::chrono::high_resolution_clock::time_point t0;                          // timestamp
    void tic() { t0 = std::chrono::high_resolution_clock::now(); }              // current time t0; "start"
    double toc_ms() const {                                                     // ms elapsed
        auto t1 = std::chrono::high_resolution_clock::now();                    // current time t1
        return std::chrono::duration<double, std::milli>(t1 - t0).count();      // duration = t1 - t0 ; in ms (float)
    }
};


// Example usage
/*
Timer T;
T.tic();

// ... some code to time ...

double ms = T.toc_ms();
std::cout << "Elapsed: " << ms << " ms\n";
*/

// high_resolution_clock is precise, but not guaranteed to be steady/monotonic
// for interval timing that must not be affected by system clock changes, use std::chrono::steady_clock
    // using clock_t = std::chrono::steady_clock;
    // ... clock_t::time_point ... clock_t::now()