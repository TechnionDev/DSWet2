#include <stdio.h>

#include <string>

#include "helperTest.h"

TEST(HashMap, Basic) {
    // TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    ASSERT_TRUE(map.isEmpty());
    int key = 7896234;  // TODO: Change to 7896234
    map.set(key, 87);
    ASSERT_EQ(map[key], 87LL);
    // TODO: TEST_TIMEOUT_FAIL_END(TIME_UNIT * 15);
}

TEST(HashMap, LotsOfAdds) {
    TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    for (int i = 0; i < COUNT * 10; i++) {
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

TEST(HashMap, AddRemoveRepeat) {
    TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    for (int i = 0; i < COUNT * 10; i++) {
        map.set(i, i * i);
        ASSERT_EQ(map.get(i), i * i);
        map.remove(i);
        ASSERT_TRUE(map.isEmpty());
    }

    ASSERT_TRUE(map.isEmpty());

    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}

TEST(HashMap, AddThenRemove) {
    // TEST_TIMEOUT_BEGIN;
    HashMap<long long> map;
    for (int i = 0; i < COUNT * 10; i++) {
        map.set(i, i * i);
        ASSERT_EQ(map.get(i), i * i);
    }

    for (int i = 0; i < COUNT * 10; i++) {
        map.remove(i);
        ASSERT_ANY_THROW(map.get(i));
    }

    ASSERT_TRUE(map.isEmpty());

    // TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}