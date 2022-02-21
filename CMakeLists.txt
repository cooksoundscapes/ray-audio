cmake_minimum_required(VERSION 3.7...3.22)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# set the project name
project(Displayer 
        VERSION 1.0
        DESCRIPTION "Lightweight, multi platform audio front-end"
        LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(Displayer include)
include_directories(Displayer include/rapidxml-1.13)

# add the executable
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(display ${SRC_FILES})

target_link_libraries(display jack lo SDL2 cairo)


