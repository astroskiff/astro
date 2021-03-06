cmake_minimum_required (VERSION 3.8)

project(astro)

set(CMAKE_CXX_STANDARD 20)

#
# Options
#
option(COMPILE_TESTS "Execute unit tests" ON)
option(WITH_ASAN     "Compile with ASAN" OFF)

#
# Setup build type 'Release vs Debug'
#
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

#
# Locate CPPUTest
#
if(DEFINED ENV{CPPUTEST_HOME})
    message(STATUS "Using CppUTest home: $ENV{CPPUTEST_HOME}")
    set(CPPUTEST_INCLUDE_DIRS $ENV{CPPUTEST_HOME}/include)
    set(CPPUTEST_LIBRARIES $ENV{CPPUTEST_HOME}/lib)
    set(CPPUTEST_LDFLAGS CppUTest CppUTestExt)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(CPPUTEST REQUIRED cpputest>=3.8)
    message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")
endif()

#
# Setup ASAN
#
if(WITH_ASAN)
  set(CMAKE_CX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

set(COMPILER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/front/lexer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/front/parser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/front/tokens.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/middle/analysis/scoped_symbol_table.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/middle/analysis/semantic_analysis.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/middle/analysis/expression_eval.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/middle/ir.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/middle/middle.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/shared/page.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/shared/reporter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/compiler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/compiler/ast.cpp
)

set(ASTRO_SOURCES
  ${COMPILER_SOURCES}
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR/compiler/front})
include_directories(${CMAKE_CURRENT_SOURCE_DIR/compiler})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        ${ASTRO_SOURCES}
        astrod.cpp)

#
# Tests
#
if(COMPILE_TESTS)
  add_subdirectory(tests)
endif()