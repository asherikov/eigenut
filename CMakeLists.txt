# General parameters
cmake_minimum_required (VERSION 2.8.12)
project (eigenut CXX)


# --------------
# Detect catkin
# --------------
if(CATKIN_DEVEL_PREFIX)
    message(STATUS "Building with catkin.")
    set(CPPUT_CATKIN_BUILD ON)
endif()
# --------------


# --------------
# Options
# --------------
set(CMAKE_VERBOSE_MAKEFILE      ON)
set(CMAKE_VERBOSE_MAKEFILE     ON)
if (EIGENUT_CATKIN_BUILD)
    set(EIGENUT_ID  "EIGENUT")
    if(CATKIN_ENABLE_TESTING)
        set(EIGENUT_BUILD_TESTS ON)
    endif()
else()
    set(EIGENUT_ID  "EIGENUT" CACHE STRING "Overrides header guards, namespace, installation path (does not apply to 3rdparty headers).")
    set(EIGENUT_SELECT_HEADERS       "" CACHE STRING "Selection a subset of headers for installation.")
    option(EIGENUT_BUILD_TESTS       "Build tests" ON)
endif()
# --------------


# --------------
# Process options
# --------------
string(TOLOWER "${EIGENUT_ID}" EIGENUT_ID_LOWER_CASE)
if (NOT ${EIGENUT_ID} STREQUAL "EIGENUT")
    set(EIGENUT_EMBEDDED ON)
endif()

if (EIGENUT_EMBEDDED)
    set(EIGENUT_BUILD_TESTS          OFF)
endif()
# --------------


# --------------
# Load cmake modules
# --------------
include(CMakeParseArguments)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
# --------------


# --------------
# Dependencies
# --------------
# Eigen
find_package(Eigen3 REQUIRED)
include_directories (SYSTEM "${EIGEN3_INCLUDE_DIR}")

# cpput
set(CPPUT_ID "${EIGENUT_ID}"  CACHE STRING "")
set(CPPUT_SELECT_HEADERS "cpput_config.h;exception.h;visibility.h"  CACHE STRING "")
option(CPPUT_BUILD_TESTS OFF)
add_subdirectory(cpput)
get_property(CPPUT_INCLUDE_DIRECTORIES DIRECTORY "cpput" PROPERTY INCLUDE_DIRECTORIES)
include_directories(${CPPUT_INCLUDE_DIRECTORIES})
# --------------


# --------------
# Configure headers
# --------------
include(cmakeut_list_filenames)

cmakeut_list_filenames("${PROJECT_SOURCE_DIR}/include/eigenut/" "EIGENUT_HEADERS")
foreach (EIGENUT_HEADER ${EIGENUT_HEADERS})
    if (${EIGENUT_HEADER} MATCHES ".h.in\$")
        string(REPLACE ".h.in" ".h" EIGENUT_HEADER_NO_SUFFIX "${EIGENUT_HEADER}")
        message("Configuring header ${EIGENUT_HEADER} -> ${EIGENUT_HEADER_NO_SUFFIX}")
        configure_file( "${PROJECT_SOURCE_DIR}/include/eigenut/${EIGENUT_HEADER}"
                        "${PROJECT_SOURCE_DIR}/include/eigenut/${EIGENUT_HEADER_NO_SUFFIX}")
    endif()
endforeach()

include_directories ("${PROJECT_SOURCE_DIR}/include")
if (EIGENUT_EMBEDDED)
    include_directories ("${PROJECT_SOURCE_DIR}/include/eigenut")
endif()
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
# Packaging
# --------------
if (EIGENUT_CATKIN_BUILD)
    find_package(catkin REQUIRED)
    catkin_package()
    set(CMAKEUT_INSTALL_PATH "${CATKIN_PACKAGE_SHARE_DESTINATION}")
else()
    set(PROJECT_VERSION_MAJOR 1)
    set(PROJECT_VERSION_MINOR 0)
    set(PROJECT_VERSION_PATCH 0)

    if (NOT EIGENUT_EMBEDDED)
        include(CMakePackageConfigHelpers)
        set(EIGENUT_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/share/eigenut/")
        set(EIGENUT_INCLUDES "${CMAKE_INSTALL_PREFIX}/include")

        configure_package_config_file(  "cmake/eigenutConfig.cmake.in"
                                        "${PROJECT_BINARY_DIR}/eigenutConfig.cmake"
                                        INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}/share/eigenut/"
                                        NO_SET_AND_CHECK_MACRO
                                        NO_CHECK_REQUIRED_COMPONENTS_MACRO)

        write_basic_package_version_file(
            ${PROJECT_BINARY_DIR}/eigenutConfigVersion.cmake
            VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
            COMPATIBILITY SameMajorVersion)


        install (FILES "${PROJECT_BINARY_DIR}/eigenutConfig.cmake"
                 DESTINATION "${EIGENUT_INSTALL_PATH}/")

        install (FILES "${PROJECT_BINARY_DIR}/eigenutConfigVersion.cmake"
                 DESTINATION "${EIGENUT_INSTALL_PATH}/")
    endif()
endif()
# --------------


# --------------
# Install
# --------------
if (EIGENUT_CATKIN_BUILD)
    set(EIGENUT_INCLUDE_INSTALL_DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
else()
    set(EIGENUT_INCLUDE_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${EIGENUT_ID_LOWER_CASE})
endif()

if (EIGENUT_SELECT_HEADERS)
    install (DIRECTORY "${PROJECT_SOURCE_DIR}/include/eigenut"
             DESTINATION "${EIGENUT_INCLUDE_INSTALL_DESTINATION}"
             FILES_MATCHING PATTERN "*.h")
else()
    foreach (EIGENUT_HEADER ${EIGENUT_SELECT_HEADERS})
        install (FILES "${PROJECT_SOURCE_DIR}/include/eigenut/${EIGENUT_HEADER}"
                 DESTINATION "${EIGENUT_INCLUDE_INSTALL_DESTINATION}/${EIGENUT_HEADER}"
                 FILES_MATCHING PATTERN "*.h")
    endforeach()
endif()
# --------------
