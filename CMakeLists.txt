cmake_minimum_required(VERSION 3.12)
project(TreeUnitTests)

set(CMAKE_CXX_STANDARD 11)
enable_testing()
find_package(GTest REQUIRED)
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)

set(CMAKE_TEST_DEBUG "-std=c++11 -Wall")
set(CMAKE_TEST_COVERAGE "${CMAKE_TEST_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_TEST_RELEASE "${CMAKE_TEST_DEBUG} -DNDEBUG")

set(CMAKE_CXX_FLAGS ${CMAKE_TEST_DEBUG})

#add_library(LinkedList LinkedList.cpp)
#add_library(Array Array.cpp)

file(GLOB_RECURSE TEST_FILES
"./unittests/test_*.cpp"
)

add_executable(TreeUnitTests ${TEST_FILES})
target_link_libraries(TreeUnitTests gtest gtest_main pthread)


file(GLOB_RECURSE MANAGER_FILES
"./Course*.cpp"
"./Lecture*.cpp"
"./List*.cpp"
)

add_executable(ManagerUnitTests ${TEST_FILES} ${MANAGER_FILES})
target_link_libraries(ManagerUnitTests gtest gtest_main pthread)

# set(CMAKE_CXX_FLAGS ${CMAKE_TEST_COVERAGE})
# add_executable(UnitTestsCoverage ${TEST_FILES})
# target_link_libraries(UnitTestsCoverage gtest gtest_main pthread)


# gtest_add_tests(BinTree "" AUTO)


# target_link_libraries(TreeUnitTests ${GTEST_LIBRARIES} gtest gtest_main pthread)
