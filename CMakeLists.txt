cmake_minimum_required(VERSION 3.20)
project(61120-MITScript)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/interpreter_joseph)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_MODE)
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
else()
    if(CMAKE_BUILD_TYPE STREQUAL "MemDebug")
        add_definitions(-DRELEASE_MODE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Os")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Os")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -flto -ffunction-sections -fdata-sections -Wl,-gc-sections")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -ffunction-sections -fdata-sections -Wl,-gc-sections")
        add_definitions(-DRELEASE_MODE)
    endif()
endif()


add_subdirectory(interpreter)
add_subdirectory(vm)
add_subdirectory(gc)

option(BUILD_TESTS "Build the unit tests" OFF)

if (${BUILD_TESTS})
    add_subdirectory(test)
    enable_testing()
endif ()

message(STATUS "Current Optimization Flags:")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
