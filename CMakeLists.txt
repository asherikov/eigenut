# General parameters
cmake_minimum_required (VERSION 2.8.12)
project (eigenut CXX)


# --------------
# Options
# --------------
set(CMAKE_VERBOSE_MAKEFILE      ON)
set(EIGENUT_BUILD_TESTS         ON)
# --------------


# --------------
# Load cmake modules
# --------------
include(CMakeParseArguments)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
# --------------


# --------------
# cpput
# --------------
set(CPPUT_ID "EIGENUT"  CACHE STRING "")
set(CPPUT_SELECT_HEADERS "cpput_config.h;exception.h;visibility.h"  CACHE STRING "")
option(CPPUT_BUILD_TESTS OFF)
add_subdirectory(cpput)
get_property(CPPUT_INCLUDE_DIRECTORIES DIRECTORY "cpput" PROPERTY INCLUDE_DIRECTORIES)
include_directories(${CPPUT_INCLUDE_DIRECTORIES})
# --------------


# --------------
# Headers
# --------------
find_package(Eigen3 REQUIRED)
include_directories (SYSTEM "${EIGEN3_INCLUDE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/include")
# --------------


# --------------
# Tests
# --------------
if(EIGENUT_BUILD_TESTS)
    enable_testing()
    add_subdirectory("${PROJECT_SOURCE_DIR}/test")
endif()
# --------------


# --------------
# Install
# --------------
install (DIRECTORY "${PROJECT_SOURCE_DIR}/include/eigenut"
         DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigenut/"
         FILES_MATCHING PATTERN "*.h")
# --------------
