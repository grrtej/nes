cmake_minimum_required(VERSION 4.0)

project(nes)

find_package(OpenGL REQUIRED)

add_subdirectory(nes/)

add_subdirectory(external/)