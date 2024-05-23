cmake_minimum_required(VERSION 3.27)
project(tatlin)

set(CMAKE_CXX_STANDARD 20)

add_executable(magnet_tape
        lib/TapeInterface.h
        src/SortTape.cpp
        lib/SortTape.h
        main.cpp
        src/Util.cpp
        lib/Util.h
        src/FileTape.cpp
        lib/FileTape.h)

add_executable(tests
        tests/tests.cpp
        lib/SortTape.h
        src/SortTape.cpp
        lib/Util.h
        src/Util.cpp
        lib/FileTape.h
        src/FileTape.cpp)