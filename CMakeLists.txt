cmake_minimum_required(VERSION 3.16)
project(uthread LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(uthread
    src/uthread.cpp
    src/scheduler.cpp
    src/context.cpp
    src/sync.cpp
    src/ipc.cpp
)

target_include_directories(uthread
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/internal
)

add_executable(demo examples/demo.cpp)
target_link_libraries(demo uthread)