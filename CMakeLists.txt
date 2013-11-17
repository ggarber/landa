cmake_minimum_required (VERSION 2.6)
project (LANDA)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wnon-virtual-dtor -stdlib=libc++")
set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

add_library (Landa src/Actor.cc src/Executor.cc)

include_directories (src)

add_executable (LandaTest test/Main.cc)
target_link_libraries (LandaTest Landa) 
