# RayTracing

## Overview
This project implements a GPU-accelerated ray tracer using CUDA alongside CPU baselines written in C++ with single-threaded and multithreaded implementations, inspired by [Ray Tracing in One Weekend][https://raytracing.github.io/books/RayTracingInOneWeekend.html] by Peter Shirley, Trevor David Black and Steve Hollasch. The goal is to explore and benchmark parallel programming, memory architecture, and high-performance computing (HPC) concepts while building a feature-rich ray tracer.

Rendering Benchmarking Results for 8K Image
<table>
  <thead>
    <tr>
      <th>Implementation</th>
      <th style="text-align:right">Runtime (ms)</th>
      <th style="text-align:right">Speedup vs Baseline</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>CPU Baseline</td>
      <td style="text-align:right">123.91</td>
      <td style="text-align:right">1.0×</td>
    </tr>
    <tr>
      <td>CPU Multi-threaded</td>
      <td style="text-align:right">34.91</td>
      <td style="text-align:right">3.5×</td>
    </tr>
    <tr>
      <td>GPU CUDA</td>
      <td style="text-align:right">3.49</td>
      <td style="text-align:right"><b>35.5×</b></td>
    </tr>
  </tbody>
</table>

## Current Features
- **Gradient image rendering**
  - CPU single-threaded baseline
  - CPU multithreaded version using `std::thread`
  - GPU-accelerated version using CUDA kernels
- **Benchmarking framework**
  - Execution time measurements with a custom `Timer`
  - Comparisons across CPU baseline, multithreaded, and GPU executions
  - Demonstrated 35× GPU speedup over CPU baseline on an 8K image
- **Image output**
  - Exports both PPM and JPG formats via `stb_image_write`
- **Memory model**
  - GPU unified memory allocation (`cudaMallocManaged`)
  - CPU copies via `std::memcpy` into standard image objects

## Project Contents
- `rag_flask_ollama.ipynb`
- `rag_flask_ollama_env.yaml`
- `data/`
- `.env`
- `.gitignore`

## Planned Extensions
Upcoming features to expand the ray tracer:
- Sphere intersection  
- Camera implementation  
- Diffuse and specular materials  
- Reflection and refraction  
- Metal and dielectric surfaces  
- Defocus blur (depth of field)  
- Motion blur  
- Texture mapping  
- Perlin noise  
- Rectangles, boxes, and volumes  
- Transformations and instancing  
- Bounding volume hierarchies (BVH)  
- Sampling optimizations  
- Multi-sample anti-aliasing  
- Monte Carlo path tracing  
- Parallelization improvements (SoA layout, SIMD on CPU, multi-GPU or distributed rendering)
- ... and more!

## Setup Instructions
1. Install CUDA Toolkit 12.4+ and Visual Studio 2022 (with MSVC v143 toolset)
2. Clone this repository
3. Build using CMake:
   ```bash
   cmake -S . -B build
   cmake --build build --config Release
4. Run executables (RayTracing.exe, ShortP3.exe, RayTracingCUDA.exe)

## Contact
If there are any questions or suggestions for improving this repository, please do not hesitate to contact me at joh78@gatech.edu.
Thank you.