cmake_minimum_required(VERSION 3.5)
project(mtsearch)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")  
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
endif()

set(Boost_USE_STATIC_LIBS true)
find_package(Boost COMPONENTS filesystem unit_test_framework REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

set(SOURCES 
    CommandLine.h
    CommandLine.cpp
    FileReading.h
    FileReading.cpp
    Finding.h
    Finding.cpp
    Matching.h
    Matching.cpp   
    Utils.h
    Utils.cpp
    )

add_executable(mtsearch ${SOURCES} main.cpp)
target_link_libraries(mtsearch PUBLIC ${Boost_LIBRARIES})

enable_testing()
set(TEST_SOURCES    
    TestCommandLine.cpp
    TestFileReading.cpp
    TestFinding.cpp
    TestMatching.cpp)

add_executable(tests ${TEST_SOURCES} testmain.cpp)
target_link_libraries(tests PUBLIC ${Boost_LIBRARIES})
add_dependencies(tests mtsearch)
add_test(NAME Tests COMMAND tests)


