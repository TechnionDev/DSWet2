#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "helperTest.h"

#define ARR_TIME_UNIT (TIME_UNIT * 10)

const int ARRAY_SIZE = 10000000;

TEST(Array, InitBigArray) {
    TEST_TIMEOUT_BEGIN;
    Array<long long> &arr = *(new Array<long long>(ARRAY_SIZE));
    ASSERT_EQ(arr[ARRAY_SIZE / 2], 0LL);
    arr[0] = 50;
    ASSERT_EQ(arr[0], 50LL);
    delete &arr;
    TEST_TIMEOUT_FAIL_END(ARR_TIME_UNIT * 6);
}

TEST(Array, InitBigArrayLoop) {
    TEST_TIMEOUT_BEGIN;
    for (int i = 0; i < COUNT; i++) {
        Array<long long> &arr = *(new Array<long long>(ARRAY_SIZE));
        ASSERT_EQ(arr[i], 0LL);
        arr[i] = i * i;
        ASSERT_EQ(arr[i], (long long)(i * i));
        delete &arr;
    }
    TEST_TIMEOUT_FAIL_END(ARR_TIME_UNIT * 6 * COUNT);
}

TEST(Array, AllocAllElements) {
    TEST_TIMEOUT_BEGIN;
    Array<long long> &arr = *(new Array<long long>(ARRAY_SIZE));

    for (int i = 0; i < ARRAY_SIZE; i++) {
        ASSERT_EQ(arr[i], 0LL);
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = i * i;
        ASSERT_EQ(arr[i], (long long)(i * i));
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        ASSERT_EQ(arr[i], (long long)(i * i));
    }

    delete &arr;
    TEST_TIMEOUT_FAIL_END(ARR_TIME_UNIT * ARRAY_SIZE);
}

TEST(Array, CheckNormal) {
    TEST_TIMEOUT_BEGIN;
    long long *arr = new long long[ARRAY_SIZE]();
    ASSERT_EQ(arr[0], 0LL);
    arr[0] = 50LL;
    ASSERT_EQ(arr[0], 50LL);
    delete[] arr;
    // Expect to timeout when using normal init array
    TEST_TIMEOUT_SUCCESS_END(ARR_TIME_UNIT * 2 * 100);
}

TEST(Array, GetConstAndDefault) {
    TEST_TIMEOUT_BEGIN;
    long long dflt = 3;
    Array<long long> &arr = *(new Array<long long>(ARRAY_SIZE, dflt));
    ASSERT_EQ(arr.length(), ARRAY_SIZE);

    for (int i = 0; i < COUNT / 100; i++) {
        ASSERT_EQ(arr.get(i), dflt);
        ASSERT_EQ(arr.used(), 0LL);
    }

    // Change default and try again
    dflt = 1;
    for (int i = 0; i < COUNT / 100; i++) {
        ASSERT_NE(arr.get(i), dflt);
        ASSERT_EQ(arr.used(), 0LL);
    }

    int c = 0;
    for (auto it : arr) {
        FAIL() << "Iteration succeeded on an empty array";
    }

    delete &arr;

    TEST_TIMEOUT_FAIL_END(ARR_TIME_UNIT * COUNT / 100);
}

TEST(Array, IteratorSimple) {
    TEST_TIMEOUT_BEGIN;
    Array<long long> &arr = *(new Array<long long>(ARRAY_SIZE));
    ASSERT_EQ(arr.length(), ARRAY_SIZE);

    for (int i = 0; i < COUNT / 100; i++) {
        ASSERT_EQ(i, arr.used());
        arr[i] = i * i;
    }

    int c = 0;
    for (auto it : arr) {
        ASSERT_EQ(c * c, it);
        c++;
    }

    delete &arr;

    TEST_TIMEOUT_FAIL_END(ARR_TIME_UNIT * COUNT / 100 * 2);
}

TEST(Array, IteratorWithAllItems) {
    TEST_TIMEOUT_BEGIN;
    Array<long long> &arr = *(new Array<long long>(COUNT));
    ASSERT_EQ(arr[0], 0LL);
    for (int i = 0; i < COUNT; i++) {
        arr[i] = i * i;
        ASSERT_EQ(arr[i], (long long)(i * i));
    }
    auto fin = arr.end();

    for (auto it = arr.begin(); it != fin; it++) {
        ASSERT_EQ((int)it, it.index());
        ASSERT_EQ(*it, arr[it]);
        ASSERT_EQ(arr[it], (long long)(((int)it) * it));
    }

    delete &arr;

    TEST_TIMEOUT_FAIL_END(ARR_TIME_UNIT * COUNT);
}

TEST(Array, IteratorWithPartItems) {
    TEST_TIMEOUT_BEGIN;
    Array<long long> &arr = *(new Array<long long>(ARRAY_SIZE));
    ASSERT_EQ(arr.length(), ARRAY_SIZE);
    ASSERT_EQ(arr[0], 0LL);
    for (int i = 0; i < COUNT; i++) {
        arr[i] = i * i;
        ASSERT_EQ(arr.used(), i + 1);
        ASSERT_EQ(arr.used(), i + 1);
        ASSERT_EQ(arr[i], (long long)(i * i));
    }
    auto fin = arr.end();
    int count = 0;
    for (auto it = arr.begin(); it != fin; count++, it++) {
        ASSERT_EQ((int)it, it.index());
        ASSERT_EQ(*it, arr[it]);
        ASSERT_EQ(arr[it], (long long)(((int)it) * it));
    }
    arr[COUNT] = 123;
    auto it = fin;
    ASSERT_EQ(it, fin);
    ASSERT_NE(it, arr.end());
    ASSERT_NE(fin, arr.end());

    auto second = arr.begin();
    it = second;

    ASSERT_EQ(second, it);
    ASSERT_EQ(*second, *it);
    ASSERT_EQ((int)second, it);

    ASSERT_EQ(count, COUNT);
    ASSERT_EQ(arr.used(), COUNT + 1);
    ASSERT_EQ(arr.length(), ARRAY_SIZE);

    delete &arr;

    TEST_TIMEOUT_FAIL_END(ARR_TIME_UNIT * COUNT);
}