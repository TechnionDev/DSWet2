#include <gtest/gtest.h>
#include <time.h>

#include <future>
#include <map>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
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
using std::vector;
using namespace LecturesStats;

// Fixtures

// Tests

const int COUNT = 5;
const int TIMEOUT = 500;  // ms

TEST(TestBinTree, InOrderInsert) {
    // TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    EXPECT_THROW(tree.get(5), NotFoundException);
    for (int i = 0; i < COUNT; i++) {
        tree.add(i, shared_ptr<int>(new int(-i)));
    }
    for (int i = 0; i < COUNT; i++) {
        EXPECT_EQ(*tree.get(i), -i);
    }
    // TEST_TIMEOUT_FAIL_END(TIMEOUT);
}
TEST(TestBinTree, ReverseOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    for (int i = COUNT; i > 0; i--) {
        tree.add(i, shared_ptr<int>(new int(-i)));
    }
    shared_ptr<int> inv_val(new int(1));
    EXPECT_THROW(tree.add(1, inv_val), AlreadyExistException);
    for (int i = COUNT; i > 0; i--) {
        EXPECT_EQ(*tree.get(i), -i);
    }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestBinTree, RandomOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<double, int> tree;
    srand(time(NULL));
    map<double, shared_ptr<int>> dict;
    double key;
    int val;
    EXPECT_THROW(tree.get(5), NotFoundException);
    for (int i = 0; i < COUNT; i++) {
        key = (double)rand() / RAND_MAX;
        val = (int)rand() % 100000;
        dict[key] = shared_ptr<int>(new int(val));
        tree.add(key, dict[key]);
    }
    EXPECT_THROW(tree.get(-1), NotFoundException);
    for (auto it : dict) {
        EXPECT_EQ(tree.get(it.first), it.second);
    }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestBinTree, InOrderInsertRandomPop) {
    // TEST_TIMEOUT_BEGIN;
    BinTree<double, int> tree;
    srand(time(NULL));
    vector<int> vec;
    int ind;
    EXPECT_THROW(tree.get(5), NotFoundException);
    for (int i = 0; i < COUNT; i++) {
        ind = (int)rand() % (vec.size() + 1);
        vec.insert(vec.begin() + ind, i);
        tree.add(i, shared_ptr<int>(new int(i * i)));
    }
    // cout << tree;
    shared_ptr<int> value;
    while (vec.size()) {
        ind = vec.back();
        vec.pop_back();
        value = tree.pop(ind);
        EXPECT_EQ(*value, ind * ind);
    }
    // TEST_TIMEOUT_FAIL_END(TIMEOUT);
}
