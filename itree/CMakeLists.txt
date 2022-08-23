cmake_minimum_required(VERSION 3.4...3.18)
project(itree)

option(LOCAL_BUILD "build locally" OFF)

if(LOCAL_BUILD)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O0")
else()
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS_DEBUG "") # -g
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
endif()


set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(Python3_EXECUTABLE ${Python3_EXECUTABLE})

add_subdirectory(pybind11 build_pybind11)

set(TARGET_SOURCES tree.cpp node.cpp node_utils.cpp common.cpp nemo.cpp uuid.cpp pickle.cpp)

pybind11_add_module(_itree py_main.cpp ${TARGET_SOURCES})

# HIQ_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(_itree PRIVATE VERSION_INFO=${HIQ_VERSION_INFO})

target_link_libraries(_itree PRIVATE pybind11::module z)


#find_package(pybind11 REQUIRED)
if(LOCAL_BUILD)
    add_executable(_itree.exe main.cpp ${TARGET_SOURCES})
    target_link_libraries(_itree.exe PRIVATE pybind11::embed z)
else()
    message("building in ci")
endif()
