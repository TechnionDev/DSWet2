#include <time.h>

#include <future>
#include <map>
#include <string>
#include <vector>

#include "helperTest.h"

// Helper functions

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> vec) {
    os << "{ ";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(os, " "));
    os << "}";
    return os;
}

// Fixtures

// Tests

const int COUNT = 1000;
const int TIMEOUT = 2000;        // ms
const int INIT_SEED = 87273654;  // For random

TEST(TestBinTree, InOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    EXPECT_EQ(tree.get(5), nullptr);
    for (int i = 0; i < COUNT; i++) {
        tree.add(i, shared_ptr<int>(new int(-i)));
    }
    for (int i = 0; i < COUNT; i++) {
        EXPECT_EQ(*tree.get(i), -i);
    }
    EXPECT_FALSE(tree.isEmpty());
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
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
    EXPECT_FALSE(tree.isEmpty());
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestBinTree, RandomOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<double, int> tree;
    srand(INIT_SEED);
    map<double, shared_ptr<int>> dict;
    double key;
    int val;
    EXPECT_EQ(tree.get(5), nullptr);
    for (int i = 0; i < COUNT; i++) {
        key = (double)rand() / RAND_MAX;
        val = (int)rand() % 100000;
        dict[key] = shared_ptr<int>(new int(val));
        tree.add(key, dict[key]);
    }
    EXPECT_EQ(tree.get(-1), nullptr);
    for (auto it : dict) {
        EXPECT_EQ(tree.get(it.first), it.second);
    }
    EXPECT_FALSE(tree.isEmpty());
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestBinTree, InOrderInsertRandomPop) {
    TEST_TIMEOUT_BEGIN;

    for (int seed = INIT_SEED; seed < 20 + INIT_SEED; seed++) {
        BinTree<int, int> tree;
        srand(seed);
        vector<int> vec;
        int ind;
        EXPECT_EQ(tree.get(5), nullptr);
        for (int i = 0; i < COUNT; i++) {
            ind = (int)rand() % (vec.size() + 1);
            vec.insert(vec.begin() + ind, i);
            tree.add(i, shared_ptr<int>(new int(i * i)));
        }
        shared_ptr<int> value;
        while (vec.size()) {
            ind = vec.back();
            vec.pop_back();
            value = tree.pop(ind);
            EXPECT_EQ(*value, ind * ind);
        }
        EXPECT_TRUE(tree.isEmpty());
    }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestBinTree, RandomInsertInOrderPop) {
    TEST_TIMEOUT_BEGIN;

    for (int seed = INIT_SEED; seed < 20 + INIT_SEED; seed++) {
        BinTree<int, int> tree;
        srand(seed);
        vector<int> vec;
        int ind;
        EXPECT_EQ(tree.get(5), nullptr);

        for (int i = 0; i < COUNT; i++) {
            ind = (int)rand() % (vec.size() + 1);
            vec.insert(vec.begin() + ind, i);
        }
        shared_ptr<int> value;
        for (auto it : vec) {
            tree.add(it, shared_ptr<int>(new int(-it)));
        }
        for (int i = 0; i < COUNT; i++) {
            value = tree.pop(i);
            EXPECT_EQ(*value, -i);
        }
        EXPECT_TRUE(tree.isEmpty());
    }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestBinTree, RandomInsertRandomPop) {
    TEST_TIMEOUT_BEGIN;

    for (int seed = INIT_SEED; seed < 20 + INIT_SEED; seed++) {
        BinTree<int, int> tree;
        srand(seed);
        vector<int> vec;
        int ind;
        EXPECT_EQ(tree.get(5), nullptr);

        for (int i = 0; i < COUNT; i++) {
            ind = (int)rand() % (vec.size() + 1);
            vec.insert(vec.begin() + ind, i);
        }
        for (auto it : vec) {
            tree.add(it, shared_ptr<int>(new int(-it)));
        }
        shared_ptr<int> value;
        while (vec.size()) {
            ind = vec.back();
            vec.pop_back();
            value = tree.pop(ind);
            EXPECT_EQ(*value, -ind);
        }
        EXPECT_TRUE(tree.isEmpty());
    }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

// TODO: Delete tree tests