#include "helperTest.h"

TEST(Library, AddCourseLotsOfClasses) {
    TEST_TIMEOUT_BEGIN;
    void *ds = Init();
    AddCourse(ds, 1, COUNT);
    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}

TEST(Library, AddLotsOfCourses) {
    const int CLASSES_COUNT = 1;

    void *ds = Init();
    for (int i = 0; i < COUNT; i++) {
        if (i > 5) {
            TEST_TIMEOUT_BEGIN;
            AddCourse(ds, i, CLASSES_COUNT);
            TEST_TIMEOUT_FAIL_END(TIME_UNIT * CLASSES_COUNT * 5 * LOG(i));
        } else {
            AddCourse(ds, i, CLASSES_COUNT);
        }
    }
}
