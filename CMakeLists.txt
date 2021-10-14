cmake_minimum_required(VERSION 3.5)
project(lvgl_wayland_tuto LANGUAGES C)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(GCC_WAYLAND_LINK_FLAGS    "-lwayland-client -lxkbcommon -lpthread")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE INCLUDES "lv_drivers/*.h" "lvgl/*.h"  "./*.h" )
file(GLOB_RECURSE SOURCES  "lv_drivers/*.c" "lvgl/*.c" )

SET(CMAKE_CXX_FLAGS "-O3")

add_executable(lvgl_wayland_tuto main.c ${SOURCES} ${INCLUDES})
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(lvgl_wayland_tuto PRIVATE ${GCC_WAYLAND_LINK_FLAGS})
