cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

file(GLOB BEARLANG_CORE_SOURCE  "src/core/*.c")
file(GLOB BEARLANG_BLI_SOURCE   "src/bli/*.c")

set(BEARLANG_BLI_SOURCE ${BEARLANG_BLI_SOURCE})

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(bearlangcore ${BEARLANG_CORE_SOURCE})

add_executable(bli ${BEARLANG_BLI_SOURCE})
target_link_libraries(bli bearlangcore readline)


