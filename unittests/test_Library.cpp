#include "helperTest.h"

const int CLASSES_COUNT = 200;  // ms

TEST(Library, AddCourseLotsOfClasses) {
    TEST_TIMEOUT_BEGIN;
                          void* ds = Init();
                          AddCourse(ds, 1, COUNT);
                          EXPECT_EQ(AddCourse(NULL, 1, COUNT), INVALID_INPUT);
                          Quit(&ds);
    TEST_TIMEOUT_FAIL_END(TIME_UNIT * COUNT);
}

TEST(Library, AddLotsOfCourses) {
    const int CLASSES_COUNT = 1;
    TEST_TIMEOUT_BEGIN;
                          void* ds = Init();
                          for (int i = 0; i < COUNT; i++) {
                              AddCourse(ds, i, CLASSES_COUNT);
                          }
                          Quit(&ds);
                          ASSERT_EQ(ds, nullptr);
    TEST_TIMEOUT_FAIL_END(
            2 * (CLASSES_COUNT * COUNT + COUNT * (LOG(COUNT) - 1)) * TIME_UNIT);
}

TEST(Library, RemoveCourseLotsOfClasses) {
    TEST_TIMEOUT_BEGIN;
                          void* ds = Init();
                          EXPECT_EQ(AddCourse(NULL, 1, COUNT), INVALID_INPUT);
                          AddCourse(ds, 1, COUNT);
                          RemoveCourse(ds, 1);
                          Quit(&ds);
    TEST_TIMEOUT_FAIL_END(2 * TIME_UNIT * COUNT * LOG(COUNT));
}

TEST(Library, RemoveCourseLotsOfCourses) {
    TEST_TIMEOUT_BEGIN;
                          void* ds = Init();
                          for (int i = 1; i < COUNT; i++) {
                              EXPECT_EQ(AddCourse(ds, i, CLASSES_COUNT), SUCCESS);
                          }
                          for (int i = 1; i < COUNT; i++) {
                              EXPECT_EQ(RemoveCourse(ds, i), SUCCESS);
                          }
                          Quit(&ds);
    TEST_TIMEOUT_FAIL_END(((CLASSES_COUNT * COUNT + COUNT * (LOG(COUNT) - 1)) +
                           CLASSES_COUNT * (CLASSES_COUNT * COUNT *
                                            (LOG(CLASSES_COUNT * COUNT) - 1))) *
                          TIME_UNIT);
}

TEST(Library, WatchClassLotsOfCourses) {
    TEST_TIMEOUT_BEGIN;
                          void* ds = Init();
                          int time;
                          EXPECT_EQ(AddCourse(ds, 1, COUNT),SUCCESS);
                          EXPECT_EQ(WatchClass(NULL, 1, 1, 2020), INVALID_INPUT);
                          EXPECT_EQ(WatchClass(ds, 1, 1, 2020), SUCCESS);
                          EXPECT_EQ(TimeViewed(ds, 1, 1, &time), SUCCESS);
                          EXPECT_EQ(time, 2020);
                          Quit(&ds);
    TEST_TIMEOUT_FAIL_END((COUNT + LOG(COUNT) + 2020) * TIME_UNIT);
}


TEST(Library, TopClass) {
//    TEST_TIMEOUT_BEGIN;
                          void* ds = Init();
                          int time;
                          EXPECT_EQ(AddCourse(ds, 5, 10), SUCCESS);
                          int courses[1] = {0};
                          int class_arr[1] = {0};
                          EXPECT_EQ(GetMostViewedClasses(NULL, 1, courses, class_arr), INVALID_INPUT);
                          EXPECT_EQ(GetMostViewedClasses(ds,1,courses, class_arr), SUCCESS);
                          EXPECT_EQ(class_arr[0], 0);
                          EXPECT_EQ(courses[0], 5);
                          EXPECT_EQ(WatchClass(ds, 5, 1, 2020), SUCCESS);
                          EXPECT_EQ(TimeViewed(ds, 5, 1, &time), SUCCESS);
                          EXPECT_EQ(time, 2020);
                          EXPECT_EQ(GetMostViewedClasses(ds,1,courses, class_arr), SUCCESS);
                          EXPECT_EQ(class_arr[0], 1);
                          EXPECT_EQ(courses[0], 5);
                          Quit(&ds);
//    TEST_TIMEOUT_FAIL_END((COUNT + LOG(COUNT) + 2020) * TIME_UNIT);
}