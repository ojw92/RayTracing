#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

// #include <memory>		// standard header for smart pointers like std::shared_ptr      // assume rtweekend.h inclusion
#include <vector>

/*
'hittable_list' is an object that contains multiple 'hittable' objects
It loops over all objects in 'objects', checking each 'object' of 'hittable' type whether
the ray hits it within the current best range, and keeps the closest hit found
Ultimately, this allows the rest of the renderer to treat the whole world as just another 'hittable',
i.e., the whole world is a 'hittable_list' consisting of multiple 'hittable' 'object's 
*/


// assume rtweekend.h inclusion
// using std::make_shared;	// function to create 'shared_ptr' objects
// using std::shared_ptr;	// reference-counted smart pointer that shares ownership of an object

class hittable_list : public hittable {
public:
	std::vector<shared_ptr<hittable>> objects;

	hittable_list() {}		// ¡°initiating¡± (constructing) an empty hittable_list object before it contains anything; this allows you to write 'hittable_list x;' to create an empty hittable_list, inside which, the 'objects' vector is default-initialized as an empty vector
	hittable_list(shared_ptr<hittable> object) { add(object); }	// 'object' is a pointer pointing to a 'hittable' type; hittable_list(some_hittable_object) immediately adds a pointer to 'objects' vector
		// hittable_list world(sphere_ptr); would create a hittable_list that already contains the sphere; world.objects contains the pointer 'sphere_ptr'

	void clear() { objects.clear(); }

	void add(shared_ptr<hittable> object) {		// add() takes one shared_ptr<hittable> and pushes it into the vector 'objects'
		objects.push_back(object);
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		hit_record temp_rec;	// temporary record to hold a candidate object¡¯s hit info before deciding if it¡¯s the closest
		bool hit_anything = false;
		auto closest_so_far = ray_t.max;	// auto = deduce type as double type

		for (const auto& object : objects) {	// read-only reference to each shared_ptr<hittable> 'object' inside 'objects'
			if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {	// call the polymorphic 'hit' on the actual object (sphere, etc)
				hit_anything = true;			// mark that we have at least one hit
				closest_so_far = temp_rec.t;	// update the best t so far
				rec = temp_rec;					// update to closest hit details
			}
		}

		return hit_anything;
	}
};

#endif


// pointer example:
// int x = 5;
// int y = x;		// copy the value of x into y, but even if you change x, y remains the same
// int* p = &x;		// p is a pointer to an int object; &x is the address of x; p stores the address of x, and p is a pointer to x
// std::cout << *p << "\n";		// *p means "go to the address in p and look inside"; since p points to x, *p is the same as x; prints 5
// *p = 10;			// change the value at the address p points to; x now becomes 10
	// x = actual value
	// p = address of x
	// *p = "follow the pointer" and access the value inside x

// Heaps are memory areas allocated to each program - https://www.geeksforgeeks.org/operating-systems/what-is-a-memory-heap/
// Memory allocated to heaps can be dynamically allocated, unlike memory allocated to stacks - i.e., heap memory is "dynamic" memory
// Pros:
	// Heap doesn¡¯t have any limit on memory size
	// It allows you to access variables globally
	// Garbage collection runs on the heap memory to free the memory used by the object
	// The heap method is also used in the Priority Queue
// Cons:
	// It takes too much time to execute compared to the stack
	// It takes more time to compute
	// It can provide the maximum memory an OS can provide
	// Memory management is more complicated in heap memory as it is used globally

// shared_ptr example:
// int* p = new int(5);		// p points to a heap integer with value 5; you create an integer 5 on the heap -> p holds its address -> you should remember to 'delete p;' later, or you'll leak memory
	// new: allocates memory for that object on the heap(dynamic memory, long-term storage)
	// Unlike local variables(stack memory), things created with 'new' live until you explicitly delete them
	// So new int(5) creates an int object in heap memory and returns a pointer to it; p points to a heap integer with value 5
// std::shared_ptr<int> x = std::make_shared<int>(5);	// 'std::make_shared<int>(5)' creates an integer (5) on the heap, but it¡¯s managed automatically, and x is a smart pointer to that integer
// std::shared_ptr<int> y = x;							// copy x into y; means now both x and y point to the same integer
// std::cout << *x << " " << *y << "\n";				// prints 5 5

// shared_ptr<type> is a pointer to some allocated type, with reference-counting semantics
// Every time you assign its value to another shared pointer (usually with a simple assignment), the reference count is incremented
// As shared pointers go out of scope (like at the end of a block or function), the reference count is decremented
// Once the count goes to zero, the object is safely deleted
// Typically, a shared pointer is first initialized with a newly - allocated object, something like this:
	// shared_ptr<double> double_ptr = make_shared<double>(0.37);
	// shared_ptr<vec3>   vec3_ptr = make_shared<vec3>(1.414214, 2.718281, 1.618034);
	// shared_ptr<sphere> sphere_ptr = make_shared<sphere>(point3(0, 0, 0), 1.0);
// make_shared<thing>(thing_constructor_params ...) allocates a new instance of type thing, using the constructor parameters. It returns a shared_ptr<thing>
// Since the type can be automatically deduced by the return type of make_shared<type>(...), the above lines can be more simply expressed using C++'s auto type specifier:
	// auto double_ptr = make_shared<double>(0.37);
	// auto vec3_ptr = make_shared<vec3>(1.414214, 2.718281, 1.618034);
	// auto sphere_ptr = make_shared<sphere>(point3(0, 0, 0), 1.0);
// Using shared pointers allows multiple geometries to share a common instance (ex. multiple spheres using the same color material) and facilitates memory management
