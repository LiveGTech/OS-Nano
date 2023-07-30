# LiveG OS Nano
# 
# Copyright (C) LiveG. All Rights Reserved.
# 
# https://liveg.tech/os
# Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.

project(OS-Nano LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(LV_CONF_PATH ${CMAKE_SOURCE_DIR}/include/lv_conf.h)

include(${CMAKE_SOURCE_DIR}/.pio/libdeps/um_feathers3/lvgl/CMakeLists.txt)

# TODO: Get EpoxyDuino to link
add_library(EpoxyDuino STATIC installdev/EpoxyDuino/cores/epoxy/main.cpp)
target_include_directories(EpoxyDuino PUBLIC installdev/EpoxyDuino/cores/epoxy)

add_executable(os-nano src/main.cpp installdev/EpoxyDuino/cores/epoxy/main.cpp)
add_definitions(-DGOSN_SIMULATOR -DEPOXY_DUINO)

target_include_directories(os-nano PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(os-nano PRIVATE ${CMAKE_SOURCE_DIR}/installdev/EpoxyDuino/cores/epoxy)
target_include_directories(os-nano PRIVATE ${CMAKE_SOURCE_DIR}/.pio/libdeps/um_feathers3/lvgl)

target_link_libraries(os-nano PRIVATE EpoxyDuino lvgl::lvgl)

file(GLOB SOURCES
    include/*.h
    src/*.cpp
)