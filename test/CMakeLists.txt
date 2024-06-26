cmake_minimum_required(VERSION 3.10.0)

project(msfit-test)

### Configure output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Print the build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()
message(STATUS "cmake build type: ${CMAKE_BUILD_TYPE}")

### Configure the compiler
# This is a basic, decent setup that should do something sane on most compilers

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

  # using Clang (linux or apple) or GCC
  message("Using clang/gcc compiler flags")
  SET(BASE_CXX_FLAGS "-std=c++17 -Wall -Wextra")
  #SET(GLIB_FLAGS "-I/opt/homebrew/Cellar/glib/2.74.5/include/glib-2.0/")
  #SET(GTK_FLAGS "`pkg-config --cflags --libs gtkmm-4.0` `pkg-config --cflags --libs glib-2.0`")
  SET(DISABLED_WARNINGS " -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-deprecated-declarations -Wno-missing-braces -Wno-unused-private-field")
  SET(TRACE_INCLUDES " -H -Wno-error=unused-command-line-argument")

  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    message("Setting clang-specific options")
    SET(BASE_CXX_FLAGS "${BASE_CXX_FLAGS} -ferror-limit=3 -fcolor-diagnostics")
    SET(CMAKE_CXX_FLAGS_DEBUG          "-g3 -fsanitize=address -fno-limit-debug-info")
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message("Setting gcc-specific options")
    SET(BASE_CXX_FLAGS "${BASE_CXX_FLAGS} -fmax-errors=5")
    SET(CMAKE_CXX_FLAGS_DEBUG          "-g3")
    SET(DISABLED_WARNINGS "${DISABLED_WARNINGS} -Wno-maybe-uninitialized -Wno-format-zero-length -Wno-unused-but-set-parameter -Wno-unused-but-set-variable")
  endif()
  
  SET(CMAKE_CXX_FLAGS "${BASE_CXX_FLAGS} ${DISABLED_WARNINGS}")

  # https://gitlab.kitware.com/cmake/cmake/-/issues/22484
  include(CheckCXXCompilerFlag)
  unset(COMPILER_SUPPORTS_MARCH_NATIVE CACHE)
  CHECK_CXX_COMPILER_FLAG(-march=native COMPILER_SUPPORTS_MARCH_NATIVE)
  if(COMPILER_SUPPORTS_MARCH_NATIVE)
    SET(CMAKE_CXX_FLAGS_RELEASE        "-O3 -march=native -DNDEBUG")
  else()
    SET(CMAKE_CXX_FLAGS_RELEASE        "-O3 -mcpu=apple-m1 -DNDEBUG") # Apple M1
  endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  message("Using Visual Studio compiler flags")
  set(BASE_CXX_FLAGS "${BASE_CXX_FLAGS} /W4")
  set(BASE_CXX_FLAGS "${BASE_CXX_FLAGS} /MP") # parallel build
  SET(DISABLED_WARNINGS "${DISABLED_WARNINGS} /wd\"4267\"")  # ignore conversion to smaller type (fires more aggressively than the gcc version, which is annoying)
  SET(DISABLED_WARNINGS "${DISABLED_WARNINGS} /wd\"4244\"")  # ignore conversion to smaller type (fires more aggressively than the gcc version, which is annoying)
  SET(DISABLED_WARNINGS "${DISABLED_WARNINGS} /wd\"4305\"")  # ignore truncation on initialization
  SET(CMAKE_CXX_FLAGS "${BASE_CXX_FLAGS} ${DISABLED_WARNINGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

  add_definitions(/D "_CRT_SECURE_NO_WARNINGS")
  add_definitions(-DNOMINMAX)
  add_definitions(-D_USE_MATH_DEFINES)
else()
  # unrecognized
  message( FATAL_ERROR "Unrecognized compiler [${CMAKE_CXX_COMPILER_ID}]" )
endif()


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0) # sets GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.68)
if (GTKMM_FOUND)
    message(STATUS "Found gtkmm-4.0")
endif()
if (GLIB_FOUND)
    message(STATUS "Found glib-2.0")
endif()
if (GLIBMM_FOUND)
    message(STATUS "Found glibmm-2.68")
endif()

# Add MsFit as a subproject
add_subdirectory(../ "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# == Build our project stuff

set(TEST_SRCS 
  src/main_test.cpp
)

add_executable(msfit-test "${TEST_SRCS}")
target_include_directories(msfit-test PRIVATE "include/")
target_include_directories(msfit-test PRIVATE ${GLIBMM_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS})

target_link_directories(msfit-test PRIVATE ${GLIBMM_LIBRARY_DIRS} ${GLIB_LIBRARY_DIRS} ${GTKMM_LIBRARY_DIRS})
target_link_libraries(msfit-test msfit ${GLIBMM_LIBRARIES} ${GLIB_LIBRARIES} ${GTKMM_LIBRARIES})

