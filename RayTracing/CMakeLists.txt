# CMakeList.txt : CMake project for RayTracing, include source and define project specific logic here.
# Build script for executables

# This is where code actually gets compiled. It tells CMake:
	# which .cpp files go into which executable
	# which headers are part of the project
	# what C++ standard to use


########### CUDA 12.2 supports Visual Studio 2022 only up to version 17.7 - current version is 17.4 ###########

cmake_minimum_required(VERSION 3.20)

# Check CUDA availability
set(CUDAToolkit_ROOT "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.4")

project(RayTracing LANGUAGES CXX)           # Only require C++

set(CMAKE_CXX_STANDARD 20)

# RayTracing executable (1st CPU-only version)
add_executable(RayTracing
    1_firstP3.cpp
    RayTracing.h
    stb_image.h
    stb_image_write.h
    vec3.h
    color.h
)

# ShortP3 executable (2nd CPU-only version)
add_executable(ShortP3
    2_shortP3.cpp
    RayTracing.h
    stb_image.h
    stb_image_write.h
    vec3.h
    color.h
)

# Try enabling CUDA
enable_language(CUDA OPTIONAL)              # CUDA is optional if nvcc fails

if (CMAKE_CUDA_COMPILER)
    message(STATUS "CUDA compiler found: ${CMAKE_CUDA_COMPILER}")

    # Override flag to allow unsupported compiler
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -allow-unsupported-compiler")

    # GPU-enabled version (CUDA)
    add_executable(RayTracingCUDA
        C_main.cu        
        C_image.h
        vec3.h
        color.h
    )
    set_target_properties(RayTracingCUDA PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
    )
else()
    message(STATUS "CUDA not found — skipping RayTracingCUDA target")
endif()





# pre-C_ commands

# Add source to this project's executable.
# CMake is not a compiler, but a build system generator. CMakeLists.txt describes the project about what source files it has, libraries it needs, where to look for headers, language standard to use, etc
#add_executable (RayTracing "1_firstP3.cpp" "RayTracing.h" "stb_image.h" "stb_image_write.h" "vec3.h" "color.h")
#add_executable (ShortP3 "2_shortP3.cpp" "RayTracing.h" "stb_image.h" "stb_image_write.h" "vec3.h" "color.h")

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET RayTracing PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.
