cmake_minimum_required (VERSION 2.8)

project (SimpleCompiler)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -std=c++11 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -std=c++11")

# header files
include_directories(${PROJECT_SOURCE_DIR}/include)
# cpp files
aux_source_directory(src/ SOURCES)

add_executable(a.out ${SOURCES})
