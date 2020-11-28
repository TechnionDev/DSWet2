cmake_minimum_required(VERSION 3.12)
project(UnitTests)

set(CMAKE_CXX_STANDARD 11)
enable_testing()
find_package(GTest REQUIRED)
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)

set(CMAKE_TEST_DEBUG "-std=c++11 -Wall")
set(CMAKE_TEST_RELEASE "${CMAKE_TEST_DEBUG} -DNDEBUG")

set(CMAKE_CXX_FLAGS ${CMAKE_TEST_DEBUG})

#add_library(LinkedList LinkedList.cpp)
#add_library(Array Array.cpp)

file(GLOB_RECURSE TEST_FILES
  "./unittests/test_*.cpp"
)

add_executable(UnitTests ${TEST_FILES})


# gtest_add_tests(BinTree "" AUTO)

target_link_libraries(UnitTests gtest gtest_main pthread)

# target_link_libraries(UnitTests ${GTEST_LIBRARIES} gtest gtest_main pthread)
