#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
    double min, max;

    // Initialize default constructor with min = +inf and max = -inf
    interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    interval(double min, double max) : min(min), max(max) {}    // if min and max are given, set min = min and max = max

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // Ensure the color components of the averaged color remains in [0,1] bounds
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x
    }

    static const interval empty, universe;  // class-level constants shared by all instances of 'interval' class object; 'global constant'
        // much like class attribute some_class.x variable in a Python class object
        // not like self.x because that's for per-object members
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);


// Notes:
// interval::empty and interval::universe are also interval types
// basically every instance of 'interval' class object has two "global constants" called 'empty' and 'universe', where
// each has class methods size(), contains() and surrounds()

#endif