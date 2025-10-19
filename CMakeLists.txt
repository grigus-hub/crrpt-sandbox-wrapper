cmake_minimum_required(VERSION 3.15)
project(CorruptJPEG C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(JPEG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${JPEG_INCLUDE_DIR})

add_library(corruptjpeg SHARED src/jpeg_api.c)
target_compile_definitions(corruptjpeg PRIVATE CORRUPTJPEG_EXPORTS)
target_link_libraries(corruptjpeg PUBLIC JPEG::JPEG)

if(WIN32)
  set_target_properties(corruptjpeg PROPERTIES
    OUTPUT_NAME "corruptjpeg"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build"
  )
endif()
