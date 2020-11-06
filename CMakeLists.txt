cmake_minimum_required(VERSION 3.17)
project(splHW1)

set(CMAKE_CXX_STANDARD 14)

add_executable(splHW1 src/main.cpp
        src/Tree.cpp
        src/Session.cpp
        src/Graph.cpp
        src/Agent.cpp
        )