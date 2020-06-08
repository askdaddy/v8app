cmake_minimum_required(VERSION 3.16)
project(v8app)
include_directories(v8/include)
link_directories(v8/obj)
link_libraries(
        v8_monolith
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -DV8_COMPRESS_POINTERS")
set(APP_SOURCE ./app/app.cpp
        ./app/utils.cpp
        ./app/expose.cpp)
add_executable(v8app main.cpp app/app.cpp app/app.h app/utils.cpp app/expose.cpp)
add_executable(shell ${APP_SOURCE})