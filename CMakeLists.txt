cmake_minimum_required(VERSION 3.20)
project(Lab2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab2
        Client.cpp
        MFQS.cpp
        MFQS.h
        RTS.cpp
        RTS.h
        Process.cpp
        Process.h
        test.cpp
        test.h)
