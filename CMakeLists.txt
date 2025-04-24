cmake_minimum_required(VERSION 3.25)

project(spake2)

set(SPAKE_VERSION_MAJOR 1)
set(SPAKE_VERSION_MINOR 0)

set(EXTERN_DIR ${CMAKE_CURRENT_LIST_DIR}/extern)

set (CMAKE_CXX_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Wall -Wextra -Wpedantic -O0 -g)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
endif()

file(WRITE ${CMAKE_CURRENT_LIST_DIR}/source/Spake2Version.hpp
  "#define SPAKE_2_MAJOR_VERSION ${SPAKE_VERSION_MAJOR}\n"
  "#define SPAKE_2_MINOR_VERSION ${SPAKE_VERSION_MINOR}\n")

include_directories(${EXTERN_DIR}/gmp/include)

if ( BUILD_TESTS )
  enable_testing()
  add_compile_definitions(CMAKE_TESTING_ENABLED)
  add_subdirectory(tests)
endif()

add_subdirectory(source)

add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/source/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} spake2_core)
