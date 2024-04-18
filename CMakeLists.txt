

project(Visualization)

find_package(
    OpenGL REQUIRED
)

add_subdirectory(src)
add_subdirectory(extern)

cmake_minimum_required(VERSION 3.11)
