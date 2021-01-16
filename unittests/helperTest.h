#include <gtest/gtest.h>

#include <time.h>
#include <future>

#include "../BinTree.h"
#include "../CoursesManager.h"
#include "../Array.h"
#include "../HashMap.h"
#include "../library.h"

#ifndef NO_TIMEOUT
#define TEST_TIMEOUT_BEGIN                             \
    std::promise<bool> promisedFinished;               \
    auto futureResult = promisedFinished.get_future(); \
                              std::thread([&](std::promise<bool>& finished) {do{}while(false)
#define TEST_TIMEOUT_FAIL_END(X)                                       \
    finished.set_value(true);                                          \
    }, std::ref(promisedFinished)).detach();                           \
    ASSERT_TRUE(futureResult.wait_for(std::chrono::microseconds(X)) != \
                std::future_status::timeout)

#define TEST_TIMEOUT_SUCCESS_END(X)                                     \
    finished.set_value(true);                                           \
    }, std::ref(promisedFinished)).detach();                            \
    ASSERT_FALSE(futureResult.wait_for(std::chrono::microseconds(X)) != \
                 std::future_status::timeout)

#else

#define TEST_TIMEOUT_BEGIN
#define TEST_TIMEOUT_SUCCESS_END(X) \
    if (false)                      \
        ;
#define TEST_TIMEOUT_FAIL_END(X) \
    if (false)                   \
        ;
#endif

#define IS_REPRESENTIBLE_IN_D_BITS(D, N)                                     \
    (((unsigned long)N >= (1UL << (D - 1)) && (unsigned long)N < (1UL << D)) \
         ? D                                                                 \
         : -1)

#define LOG_1(n) (((n) >= 2) ? 1 : 0)
#define LOG_2(n) (((n) >= 1 << 2) ? (2 + LOG_1((n) >> 2)) : LOG_1(n))
#define LOG_4(n) (((n) >= 1 << 4) ? (4 + LOG_2((n) >> 4)) : LOG_2(n))
#define LOG_8(n) (((n) >= 1 << 8) ? (8 + LOG_4((n) >> 8)) : LOG_4(n))
#define LOG_16(n) (((n) >= 1 << 16) ? (16 + LOG_8((n) >> 16)) : LOG_8(n))
#define LOG(n) ((LOG_16(n) + !!((n) & ((n)-1))) + 1)

using namespace std;
using namespace LecturesStats;

#ifndef TEST_COVERAGE
const unsigned long COUNT = 2000000;
const unsigned long RAND_COUNT = 300;
const unsigned long RAND_ITEM_COUNT = COUNT / 10;
const unsigned long TIME_UNIT = 3;  // microseconds
#else
const unsigned long COUNT = 500;
const unsigned long RAND_COUNT = 3;
const unsigned long RAND_ITEM_COUNT = COUNT / 10;
const unsigned long TIME_UNIT = 20;  // microseconds
#endif
const int INIT_SEED = 87273654;  // For random