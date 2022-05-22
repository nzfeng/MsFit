cmake_minimum_required(VERSION 3.10.0)

# Maybe stop from CMAKEing in the wrong place
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same. Go use the /build directory.")
endif()

# List of .cpp files to be compiled
SET(SRCS
  engine/word-searcher.cpp
  interface/msfit-interface.cpp
  interface/state.cpp
  interface/main-window.cpp
  interface/main-container.cpp
  interface/grid-container.cpp
  interface/right-menu-container.cpp
  interface/bottom-menu-container.cpp
  interface/canvas-container.cpp
)

SET(INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../include/msfit/")
SET(HEADERS
  ${INCLUDE_ROOT}/engine/word-searcher.h
  ${INCLUDE_ROOT}/interface/msfit-interface.h
  ${INCLUDE_ROOT}/interface/state.h
  ${INCLUDE_ROOT}/interface/main-window.h
  ${INCLUDE_ROOT}/interface/main-container.h
  ${INCLUDE_ROOT}/interface/grid-container.h
  ${INCLUDE_ROOT}/interface/right-menu-container.h
  ${INCLUDE_ROOT}/interface/bottom-menu-container.h
  ${INCLUDE_ROOT}/interface/canvas-container.h
)


# Create a single library for the project
add_library(msfit ${SRCS} ${HEADERS})

# Includes from this project
target_include_directories(msfit PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_include_directories(msfit PUBLIC ${GTKMM_INCLUDE_DIRS})

# Add all includes and link libraries from dependencies, which were populated in deps/CMakeLists.txt
target_link_libraries(msfit PUBLIC ${MSF_DEP_LIBS})

# Set compiler properties for the library
target_compile_features(msfit PUBLIC cxx_std_17)
set_target_properties(msfit PROPERTIES
  CXX_STANDARD_REQUIRED TRUE
  CXX_EXTENSIONS OFF
)
target_compile_definitions(msfit PUBLIC NOMINMAX _USE_MATH_DEFINES)
