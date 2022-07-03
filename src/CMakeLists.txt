cmake_minimum_required(VERSION 3.10.0)

# Maybe stop from CMAKEing in the wrong place
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same. Go use the /build directory.")
endif()

# List of .cpp files to be compiled
SET(SRCS
  utilities/utils.cpp
  utilities/state.cpp
  utilities/drawing_utils.cpp

  interface/msfit_interface.cpp
  interface/main_window.cpp
  interface/right_menu_container.cpp
  interface/messages_list.cpp
  interface/msfit_button.cpp

  puzzle/square.cpp
  puzzle/puzzle_grid.cpp

  engine/dataset_manager.cpp
  engine/fill_manager.cpp
  engine/word_utils.cpp
)

SET(INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../include/msfit/")
SET(HEADERS
  ${INCLUDE_ROOT}/utilities/utils.h
  ${INCLUDE_ROOT}/utilities/state.h
  ${INCLUDE_ROOT}/utilities/drawing_utils.h
  ${INCLUDE_ROOT}/utilities/vector2.h
  ${INCLUDE_ROOT}/utilities/vector2.ipp

  ${INCLUDE_ROOT}/interface/msfit_interface.h
  ${INCLUDE_ROOT}/interface/main_window.h
  ${INCLUDE_ROOT}/interface/right_menu_container.h
  ${INCLUDE_ROOT}/interface/messages_list.h
  ${INCLUDE_ROOT}/interface/msfit_button.h

  ${INCLUDE_ROOT}/puzzle/square.h
  ${INCLUDE_ROOT}/puzzle/square.ipp
  ${INCLUDE_ROOT}/puzzle/puzzle_grid.h
  ${INCLUDE_ROOT}/puzzle/puzzle_grid.ipp
  ${INCLUDE_ROOT}/puzzle/grid_word.h
  ${INCLUDE_ROOT}/puzzle/grid_word.ipp

  ${INCLUDE_ROOT}/engine/dataset_manager.h
  ${INCLUDE_ROOT}/engine/fill_manager.h
  ${INCLUDE_ROOT}/engine/word_utils.h
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtkmm REQUIRED IMPORTED_TARGET gtkmm-4.0)
#pkg_check_modules(glibmm REQUIRED IMPORTED_TARGET glibmm>=2.4)

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
