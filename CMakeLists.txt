cmake_minimum_required(VERSION 2.6)

project(ample C CXX)
set(VERSION 0.3)

find_library(verse_LIBRARY verse PATHS ../verse)
if(NOT verse_LIBRARY)
  message(FATAL_ERROR "Verse library not found")
endif(NOT verse_LIBRARY)

find_path(verse_INCLUDE verse.h PATHS ../verse)
if(NOT verse_INCLUDE)
  message(FATAL_ERROR "Verse header not found")
endif(NOT verse_INCLUDE)

configure_file(${ample_SOURCE_DIR}/docs/Doxyfile.cmake
               ${ample_BINARY_DIR}/docs/Doxyfile @ONLY)

include_directories(include ${verse_INCLUDE})

add_subdirectory(src)
add_subdirectory(examples)

install(DIRECTORY include
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")

