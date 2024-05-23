cmake_minimum_required(VERSION 3.27)
project(tatlin)

find_package(Boost 1.65 REQUIRED COMPONENTS program_options)
find_package(Catch2 2.13.10 REQUIRED)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.10
)

FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG        boost-1.65.0
        GIT_SHALLOW    TRUE
)

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

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(magnet_tape ${Boost_LIBRARIES})
endif ()

if (Catch2_FOUND)
    include_directories(${Catch2_INCLUDE_DIRS})
endif ()

add_executable(tests
        tests/tests.cpp
        lib/SortTape.h
        src/SortTape.cpp
        lib/Util.h
        src/Util.cpp
        lib/FileTape.h
        src/FileTape.cpp)