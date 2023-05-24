cmake_minimum_required(VERSION 3.10.0)

# We will accumulate all depenency includes and link libraries here
# (can be relative paths when building list, we will convert all to absolute at end)
SET(MSF_DEP_LIBS "")
SET(MSF_DEP_LIBS_DIRS "")

# Make sure we don't try to compile anything in the dependencies with Werror on
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error")
endif()

# === gtkmm
message("--  resolving gtkmm dependency:")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

# Propagate dep variables up to parent
SET(MSF_DEP_LIBS ${MSF_DEP_LIBS} PARENT_SCOPE)
SET(MSF_DEP_LIBS_DIRS ${MSF_DEP_LIBS_DIRS} PARENT_SCOPE)