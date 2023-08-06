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

file(GLOB EPOXY_SOURCES
    ${CMAKE_SOURCE_DIR}/installdev/EpoxyDuino/cores/epoxy/*.cpp
    ${CMAKE_SOURCE_DIR}/installdev/EpoxyDuino/cores/epoxy/*.h
)

add_library(EpoxyDuino STATIC ${EPOXY_SOURCES})

target_include_directories(EpoxyDuino PUBLIC ${CMAKE_SOURCE_DIR}/installdev/EpoxyDuino/cores/epoxy)

add_library(duktape STATIC installdev/duktape/src/duktape.c)

file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/include/*.h
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(os-nano ${SOURCES})

add_definitions(
    -DGOSN_SIMULATOR
)

target_compile_options(os-nano PRIVATE
    -Wno-unused-command-line-argument
    -Wno-delete-incomplete
)

set_target_properties(os-nano PROPERTIES LINK_FLAGS "--bind --embed-file rootfs@/")

target_include_directories(os-nano PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(os-nano PRIVATE ${CMAKE_SOURCE_DIR}/installdev/EpoxyDuino/cores/epoxy)
target_include_directories(os-nano PRIVATE ${CMAKE_SOURCE_DIR}/.pio/libdeps/um_feathers3/lvgl)
target_include_directories(os-nano PRIVATE ${CMAKE_SOURCE_DIR}/installdev/duktape/src)

target_link_libraries(os-nano PRIVATE EpoxyDuino lvgl::lvgl duktape)

set_target_properties(lvgl_examples lvgl_demos PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)