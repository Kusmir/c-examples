cmake_minimum_required(VERSION 3.25)
project(tramway)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wshadow -Wconversion -Wno-sign-conversion \
-Wfloat-equal -D_GLIBCXX_DEBUG -ggdb3  -fsanitize=address,undefined \
-fno-omit-frame-pointer -fno-optimize-sibling-calls -DLOCAL")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(test)