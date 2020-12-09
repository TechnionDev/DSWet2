#include "helperTest.h"

TEST(Library, AddCourseLotsOfClasses) {
    TEST_TIMEOUT_BEGIN;
    void *ds = Init();
    AddCourse(ds, 1, COUNT);
    Quit(&ds);
    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}

TEST(Library, AddLotsOfCourses) {
    const int CLASSES_COUNT = 1;
    TEST_TIMEOUT_BEGIN;
    void *ds = Init();
    for (int i = 0; i < COUNT; i++) {
        AddCourse(ds, i, CLASSES_COUNT);
    }
    Quit(&ds);
    ASSERT_EQ(ds, nullptr);
    TEST_TIMEOUT_FAIL_END(
        2 * (CLASSES_COUNT * COUNT + COUNT * (LOG(COUNT) - 1)) * TIME_UNIT);
}
