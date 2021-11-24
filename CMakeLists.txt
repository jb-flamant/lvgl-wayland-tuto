cmake_minimum_required(VERSION 3.5)
project(lvgl_wayland_tuto LANGUAGES C)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(GCC_WAYLAND_LINK_FLAGS    "-lwayland-client -lwayland-cursor -lxkbcommon -lpthread")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE INCLUDES "lv_drivers/*.h" "lvgl/*.h"  "./*.h" )
file(GLOB_RECURSE SOURCES_LVGL  "lvgl/*.c" )
file(GLOB SOURCES_LVGL_DRV
    "lv_drivers/display/*.c"
    "lv_drivers/indev/*.c"
    "lv_drivers/wayland/*.c"
    "lv_drivers/wayland/protocols/*.c" )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(lvgl_wayland_tuto main.c ${SOURCES_LVGL} ${SOURCES_LVGL_DRV} ${INCLUDES})
target_link_libraries(lvgl_wayland_tuto PRIVATE ${GCC_WAYLAND_LINK_FLAGS})
