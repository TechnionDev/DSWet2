#include <time.h>

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

TEST(TestBinTree, InOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    ASSERT_EQ(tree.get(5), nullptr);
    for (int i = 0; i < COUNT; i++) {
        tree.add(i, shared_ptr<int>(new int(-i)));
    }
    for (int i = 0; i < COUNT; i++) {
        ASSERT_EQ(*tree.get(i), -i);
    }
    ASSERT_FALSE(tree.isEmpty());
    TEST_TIMEOUT_FAIL_END(LOG(COUNT) * TIME_UNIT * COUNT);
}
TEST(TestBinTree, ReverseOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    for (int i = COUNT; i > 0; i--) {
        tree.add(i, shared_ptr<int>(new int(-i)));
    }
    shared_ptr<int> inv_val(new int(1));
    ASSERT_THROW(tree.add(1, inv_val), AlreadyExistException);
    for (int i = COUNT; i > 0; i--) {
        ASSERT_EQ(*tree.get(i), -i);
    }
    ASSERT_FALSE(tree.isEmpty());
    TEST_TIMEOUT_FAIL_END(LOG(COUNT) * TIME_UNIT * COUNT);
}

TEST(TestBinTree, RandomOrderInsert) {
    TEST_TIMEOUT_BEGIN;
    BinTree<double, int> tree;
    srand(INIT_SEED);
    map<double, shared_ptr<int>> dict;
    double key;
    int val;
    ASSERT_EQ(tree.get(5), nullptr);
    for (int i = 0; i < COUNT; i++) {
        key = (double)rand() / RAND_MAX;
        val = (int)rand() % 100000;
        dict[key] = shared_ptr<int>(new int(val));
        tree.add(key, dict[key]);
    }
    ASSERT_EQ(tree.get(-1), nullptr);
    for (auto it : dict) {
        ASSERT_EQ(tree.get(it.first), it.second);
    }
    ASSERT_FALSE(tree.isEmpty());
    TEST_TIMEOUT_FAIL_END(LOG(COUNT) * TIME_UNIT * COUNT);
}

TEST(TestBinTree, InOrderInsertRandomPop) {
    TEST_TIMEOUT_BEGIN;
    for (int seed = INIT_SEED; seed < RAND_COUNT + INIT_SEED; seed++) {
        BinTree<int, int> tree;
        srand(seed);
        vector<int> vec;
        int ind;
        ASSERT_EQ(tree.get(5), nullptr);
        for (int i = 0; i < RAND_ITEM_COUNT; i++) {
            ind = (int)rand() % (vec.size() + 1);
            vec.insert(vec.begin() + ind, i);
            tree.add(i, shared_ptr<int>(new int(i * i)));
        }
        shared_ptr<int> value;
        while (vec.size()) {
            ind = vec.back();
            vec.pop_back();
            value = tree.pop(ind);
            ASSERT_EQ(*value, ind * ind);
        }
        ASSERT_TRUE(tree.isEmpty());
    }
    TEST_TIMEOUT_FAIL_END(LOG(RAND_ITEM_COUNT) * TIME_UNIT * RAND_ITEM_COUNT *
                          RAND_COUNT);
}

TEST(TestBinTree, RandomInsertInOrderPop) {
    TEST_TIMEOUT_BEGIN;

    for (int seed = INIT_SEED; seed < RAND_COUNT + INIT_SEED; seed++) {
        BinTree<int, int> tree;
        srand(seed);
        vector<int> vec;
        int ind;
        ASSERT_EQ(tree.get(5), nullptr);

        for (int i = 0; i < RAND_ITEM_COUNT; i++) {
            ind = (int)rand() % (vec.size() + 1);
            vec.insert(vec.begin() + ind, i);
        }
        shared_ptr<int> value;
        for (auto it : vec) {
            tree.add(it, shared_ptr<int>(new int(-it)));
        }
        for (int i = 0; i < RAND_ITEM_COUNT; i++) {
            value = tree.pop(i);
            ASSERT_EQ(*value, -i);
        }
        ASSERT_TRUE(tree.isEmpty());
    }
    TEST_TIMEOUT_FAIL_END(LOG(RAND_ITEM_COUNT) * TIME_UNIT * RAND_ITEM_COUNT *
                          RAND_COUNT);
}

TEST(TestBinTree, RandomInsertRandomPop) {
    TEST_TIMEOUT_BEGIN;

    for (int seed = INIT_SEED; seed < RAND_COUNT + INIT_SEED; seed++) {
        BinTree<int, int> tree;
        srand(seed);
        vector<int> vec;
        int ind;
        ASSERT_EQ(tree.get(5), nullptr);

        for (int i = 0; i < RAND_ITEM_COUNT; i++) {
            ind = (int)rand() % (vec.size() + 1);
            vec.insert(vec.begin() + ind, i);
        }
        for (auto it : vec) {
            tree.add(it, shared_ptr<int>(new int(-it)));
        }
        ASSERT_EQ(tree.pop(-4), nullptr);
        shared_ptr<int> value;
        while (vec.size()) {
            ind = vec.back();
            vec.pop_back();
            value = tree.pop(ind);
            ASSERT_EQ(*value, -ind);
        }
        ASSERT_TRUE(tree.isEmpty());
        ASSERT_EQ(tree.pop(1), nullptr);
    }
    TEST_TIMEOUT_FAIL_END(LOG(RAND_ITEM_COUNT) * TIME_UNIT * RAND_ITEM_COUNT *
                          RAND_COUNT);
}

TEST(TestBinTree, TreeIteratorSecondRise) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    tree.add(0, shared_ptr<int>(new int(0)));
    tree.add(1, shared_ptr<int>(new int(0)));
    auto it = tree.begin();
    ASSERT_NE(it++, tree.end());
    ASSERT_NE(it, tree.end());
    ASSERT_NE(it++, tree.end());
    ASSERT_EQ(it, tree.end());
    TEST_TIMEOUT_FAIL_END(TIME_UNIT * 60);
}

TEST(TestBinTree, TreeIterator) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree = BinTree<int, int>();
    for (int i = COUNT; i > 0; i--) {
        tree.add(i, shared_ptr<int>(new int(-i)));
    }
    shared_ptr<int> inv_val(new int(1));
    ASSERT_THROW(tree.add(1, inv_val), AlreadyExistException);
    {
        int i = COUNT;
        for (auto it = tree.begin(); it != tree.end(); i--, it++) {
            ASSERT_EQ(it.key(), i);
            ASSERT_EQ(*it.value(), -i);
        }
    }
    {  // Remove one from the middle and re test iterations
        int i = COUNT;
        tree.pop(COUNT / 2);
        auto it = tree.begin();
        for (; it != tree.end(); i--, it++) {
            if (i == COUNT / 2) i--;
            ASSERT_EQ(it.key(), i);
            ASSERT_EQ(*it.value(), -i);
        }
        ASSERT_THROW(it++, OutOfBoundsException);
        try {
            it++;
        } catch (const OutOfBoundsException& e) {
            cerr << e.what() << endl;
        }
    }
    ASSERT_FALSE(tree.isEmpty());
    TEST_TIMEOUT_FAIL_END(LOG(COUNT) * TIME_UNIT * COUNT);
}

TEST(TestBinTree, TreeFromRange) {
    TEST_TIMEOUT_BEGIN;
    BinTree<int, int> tree(COUNT);
    tree.isTreeStructured();
    auto it = tree.begin();
    for (int i = COUNT - 1; i >= 0; i--, it++) {
        EXPECT_EQ(it.key(), i);
        EXPECT_EQ(it.value(), nullptr);
    }
    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}

// TODO: Delete tree tests