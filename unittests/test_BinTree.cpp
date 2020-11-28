#include <gtest/gtest.h>
#include <time.h>

#include <future>
#include <map>
#include <string>

#include "../BinTree.h"
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

// Helper functions

using std::map;
using std::string;
using namespace LecturesStats;

// Fixtures
/*
struct files_state {
    string name;
    string in_filename;
    string out_filename;
};

struct BinTreeTest : testing::Test,
                               testing::WithParamInterface<files_state> {
    string case_name;
    BinTreeTest() { case_name = GetParam().name; }
    struct PrintToStringParamName {
        template <class ParamType>
        std::string operator()(
            const testing::TestParamInfo<ParamType> &info) const {
            auto state = static_cast<files_state>(info.param);
            return state.name;
        }
    };
};
 */
/* Tests

TEST_F(BinTreeTest, TestFiles) {
    auto state = GetParam();

} */

const int COUNT = 50000;
const int TIMEOUT = 1000; // ms

TEST(TestBinTree, InOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    EXPECT_THROW(tree.get(5), NotFoundException);
    for (int i = 0; i < COUNT; i++) {
        EXPECT_NO_THROW(tree.add(i, -i));
    }
    for (int i = 0; i < COUNT; i++) {
        EXPECT_EQ(tree.get(i), -i);
    }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}
TEST(TestBinTree, ReverseOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    for (int i = COUNT; i > 0; i--) {
        EXPECT_NO_THROW(tree.add(i, -i));
    }
    EXPECT_THROW(tree.add(1, 1), AlreadyExistException);
    for (int i = COUNT; i > 0; i--) {
        EXPECT_EQ(tree.get(i), -i);
    }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestBinTree, RandomOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<double, int> tree;
    srand(time(NULL));
    map<double, int> dict;
    double key;
    int val;
    EXPECT_THROW(tree.get(5), NotFoundException);
    for (int i = 0; i < COUNT; i++) {
        key = (double)rand() / RAND_MAX;
        val = (int)rand() % 100000;
        dict[key] = val;
        EXPECT_NO_THROW(tree.add(key, val));
    }
    EXPECT_THROW(tree.get(-1), NotFoundException);
    for (auto it : dict) {
        EXPECT_EQ(tree.get(it.first), it.second);
    }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}
