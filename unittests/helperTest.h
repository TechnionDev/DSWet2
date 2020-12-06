#include <gtest/gtest.h>
#include "../CoursesManager.h"
#include "../BinTree.h"

#ifndef TEST_COVERAGE
#define TEST_TIMEOUT_BEGIN                             \
    std::promise<bool> promisedFinished;               \
    auto futureResult = promisedFinished.get_future(); \
                              std::thread([](std::promise<bool>& finished) {do{}while(false)
#define TEST_TIMEOUT_FAIL_END(X)                                       \
    finished.set_value(true);                                          \
    }, std::ref(promisedFinished)).detach();                           \
    EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(X)) != \
                std::future_status::timeout)

#define TEST_TIMEOUT_SUCCESS_END(X)                                     \
    finished.set_value(true);                                           \
    }, std::ref(promisedFinished)).detach();                            \
    EXPECT_FALSE(futureResult.wait_for(std::chrono::milliseconds(X)) != \
                 std::future_status::timeout)

#else

#define TEST_TIMEOUT_BEGIN
#define TEST_TIMEOUT_SUCCESS_END(X) if(false);
#define TEST_TIMEOUT_FAIL_END(X) if(false);
#endif


using namespace std;
using namespace LecturesStats;
