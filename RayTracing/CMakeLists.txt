# CMakeList.txt : CMake project for RayTracing, include source and define
# project specific logic here.
#

# Add source to this project's executable.
# CMake is not a compiler, but a build system generator. CMakeLists.txt describes the project about what source files it has, libraries it needs, where to look for headers, language standard to use, etc
add_executable (RayTracing "1_firstP3.cpp" "RayTracing.h" "stb_image.h" "stb_image_write.h" "vec3.h" "color.h")
add_executable (ShortP3 "2_shortP3.cpp" "RayTracing.h" "stb_image.h" "stb_image_write.h" "vec3.h" "color.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RayTracing PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
