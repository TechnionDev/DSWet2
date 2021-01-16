#include <stdio.h>

#include <string>

#include "helperTest.h"

TEST(HashMap, Basic) {
    TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    ASSERT_TRUE(map.isEmpty());
    int key = 7896234;
    map.set(key, 87);
    ASSERT_EQ(map[key], 87LL);

    TEST_TIMEOUT_FAIL_END(TIME_UNIT + 50 * TIME_UNIT);
}

TEST(HashMap, LotsOfAdds) {
    TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    for (int i = 0; i < COUNT; i++) {
        map.set(i, i * i);
    }

    for (int i = 0; i < COUNT; i += 7) {
        ASSERT_EQ(map.get(i), i * i);
    }

    for (int i = 2; i < COUNT; i += 7) {
        ASSERT_EQ(map[i], i * i);
    }

    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}

TEST(HashMap, FillDefaults) {
    TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    for (int i = 0; i < COUNT; i++) {
        map[i];
    }

    for (int i = 0; i < COUNT / 10; i += 7) {
        ASSERT_EQ(map.get(i), 0LL);
    }

    for (int i = 2; i < COUNT / 10; i += 7) {
        ASSERT_EQ(map[i], 0LL);
    }

    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}

TEST(HashMap, GetOnNothing) {
    TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    for (int i = 0; i < COUNT; i++) {
        ASSERT_ANY_THROW(map.get(i));
    }

    // x5 to account for the try/catch overhead
    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT * 5);
}

TEST(HashMap, AddRemoveRepeat) {
    TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    for (int i = 0; i < COUNT; i++) {
        map.set(i, i * i);
        ASSERT_EQ(map.get(i), i * i);
        map.remove(i);
        ASSERT_TRUE(map.isEmpty());
    }

    ASSERT_TRUE(map.isEmpty());

    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}

TEST(HashMap, AddThenRemove) {
    TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    for (int i = 0; i < COUNT; i++) {
        map.set(i, i * i);
        ASSERT_EQ(map.get(i), i * i);
    }

    for (int i = 0; i < COUNT; i++) {
        map.remove(i);
        ASSERT_FALSE(map.exist(i));
    }

    ASSERT_TRUE(map.isEmpty());

    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT * 2);
}

TEST(HashMap, StdMapAddThenRemoveBaseLine) {
    std::map<int, long long> dict;

    for (int i = 0; i < COUNT; i++) {
        dict[i] = i * i;
        ASSERT_EQ(dict[i], i * i);
    }

    for (int i = 0; i < COUNT; i++) {
        dict.erase(i);
        // ASSERT_FALSE(dict.find(i) == dict.end());
    }

    ASSERT_EQ(dict.size(), 0);
}

TEST(HashMap, RandomAddRandomRemove) {
    TEST_TIMEOUT_BEGIN;
    int key, val;
    for (int j = 0; j < RAND_COUNT * 10; j++) {
        srand(INIT_SEED + j);
        HashMap<long long> map;
        cout << "Rand iteration: " << j << endl;
        for (int i = 0; i < COUNT; i++) {
            key = (int)rand() % INT_MAX;
            val = key / 2;
            ASSERT_NO_THROW(map.set(key, val));
        }
    }

    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT * RAND_COUNT * 10);
}
