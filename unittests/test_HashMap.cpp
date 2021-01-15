#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>

#include "helperTest.h"

void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

TEST(HashMap, Basic) {
    // TEST_TIMEOUT_BEGIN;
    HashMap<long long> &map = *(new HashMap<long long>());
    ASSERT_TRUE(map.isEmpty());
    int key = 7896234;  // TODO: Change to 7896234
    map.set(key, 87);
    ASSERT_EQ(map[key], 87LL);

    delete &map;
    // TODO: TEST_TIMEOUT_FAIL_END(TIME_UNIT * 15);
}

TEST(HashMap, MultipleAdds) {
    // TEST_TIMEOUT_BEGIN;
    signal(SIGSEGV, handler);  // install our handler

    cout << "Starts" << endl;
    HashMap<long long> &map = *(new HashMap<long long>());
    cout << "Test0" << endl;
    ASSERT_TRUE(map.isEmpty());
    int key = 7896234;
    cout << "Test1" << endl;
    map.set(key, key * 2);
    cout << "Test2" << endl;
    map.set(key * 7, key * 3);
    cout << "Test3" << endl;
    ASSERT_EQ(map[key], key * 2);
    cout << "Test4" << endl;
    ASSERT_EQ(map[key * 7], key * 3);
    cout << "Test5" << endl;

    delete &map;
    // TODO: TEST_TIMEOUT_FAIL_END(TIME_UNIT * 15);
}
