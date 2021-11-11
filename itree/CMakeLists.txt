cmake_minimum_required(VERSION 3.4...3.18)
project(itree)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "") # -g
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

add_subdirectory(../pybind11 build_pybind11)
add_subdirectory(../fmt build_fmt)


set(TARGET_SOURCES tree.cpp node.cpp node_utils.cpp common.cpp nemo.cpp uuid.cpp)

pybind11_add_module(_itree py_main.cpp ${TARGET_SOURCES})

# HIQ_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(_itree PRIVATE VERSION_INFO=${HIQ_VERSION_INFO})

find_package(fmt)
target_link_libraries(_itree PRIVATE pybind11::module fmt::fmt z)

#########################################################
#find_package(pybind11 REQUIRED)
add_executable(_itree.exe main.cpp ${TARGET_SOURCES})
find_package(fmt)
target_link_libraries(_itree.exe PRIVATE pybind11::embed fmt::fmt z)
