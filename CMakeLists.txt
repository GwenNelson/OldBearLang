cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set (CMAKE_CXX_STANDARD 14)
add_compile_options(-std=c++14)

#add_subdirectory(sexp-cpp)

file(GLOB BEARLANG_SOURCE  "src/libbearlang/*.cpp")
file(GLOB SEXP_SOURCE      "sexp-cpp/src/*.cpp")
file(GLOB BEARLANGI_SOURCE "src/bearlangi/*.cpp")
file(GLOB BEARLANGC_SOURCE "src/bearlangc/*.cpp")

set(LIBSRC ${BEARLANG_SOURCE} ${SEXP_SOURCE})

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/sexp-cpp/include)

add_library(bearlang SHARED ${LIBSRC})
add_library(bearlangrt STATIC ${LIBSRC})

add_executable(bearlangi ${BEARLANGI_SOURCE})
target_link_libraries(bearlangi bearlang readline)

add_executable(bearlangc ${BEARLANGC_SOURCE})
target_link_libraries(bearlangc bearlang readline)

