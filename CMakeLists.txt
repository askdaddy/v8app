cmake_minimum_required(VERSION 3.16)
project(v8app)
include_directories(v8/include)
link_directories(v8/obj)
link_libraries(
        v8_monolith
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -DV8_COMPRESS_POINTERS")
add_executable(v8app main.cpp)