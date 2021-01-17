cmake_minimum_required(VERSION 3.12)
project(UnitTests)

set(CMAKE_CXX_STANDARD 11)
enable_testing()
find_package(GTest REQUIRED)
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)

set(CMAKE_TEST_DEBUG "-g -std=c++11 -Wall")
set(CMAKE_TEST_COVERAGE "${CMAKE_TEST_DEBUG} -DNDEBUG -fprofile-arcs -ftest-coverage")
set(CMAKE_TEST_RELEASE "${CMAKE_TEST_DEBUG} -DNDEBUG -DTEST_COVERAGE")

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g ${CMAKE_TEST_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE_INIT ${CMAKE_TEST_RELEASE})

#add_library(LinkedList LinkedList.cpp)
#add_library(Array Array.cpp)

file(GLOB_RECURSE TEST_FILES
"./unittests/test_*.cpp"
)

file(GLOB_RECURSE MANAGER_FILES
"./Course*.cpp"
"./Lecture*.cpp"
"./List*.cpp"
"./Key.cpp"
"library2.cpp"
"./Hash*"
)

add_executable(ManagerUnitTests ${TEST_FILES} ${MANAGER_FILES})
# add_executable(ManagerUnitTests ${MANAGER_FILES} unittests/TestRunner.cpp unittests/test_CourseManager.cpp)
target_link_libraries(ManagerUnitTests gtest gtest_main pthread)
