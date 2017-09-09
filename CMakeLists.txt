cmake_minimum_required (VERSION 3.0)
option(test "Build tests" OFF)

set(PROJECT_NAME wavparser)
project(${PROJECT_NAME})

add_compile_options(-std=c++11 -g -Wall)
find_package(Threads REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)

add_subdirectory(src)

if(test)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/tests/lib)

    enable_testing()
    add_subdirectory(tests)

    add_executable(wavparser_tests.out tests/main.cpp)
    target_link_libraries(wavparser_tests.out
        libgtest 
        libgmock	
        util
        audio
        audio_tests
    )

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
endif()

add_executable(wavparser src/main.cpp)
target_link_libraries(wavparser
    util
    audio
)
