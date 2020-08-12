cmake_minimum_required(VERSION 3.18)
project(dijkstra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(FetchContent)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) # Include Conan-generated file
conan_basic_setup(TARGETS) # Introduce Conan-generated targets

include_directories(/home/a/.conan/data/range-v3/0.10.0/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include)


include_directories(src)

add_subdirectory(src)

add_subdirectory(test)
add_subdirectory(lib)