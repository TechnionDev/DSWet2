#include <time.h>

#include <future>
#include <map>
#include <string>
#include <vector>

#include "helperTest.h"

const int COUNT = 1000;
const int TIMEOUT = 10000;        // ms

// Helper functions
TEST(TestCoursesManager, InsertCoourse) {
    TEST_TIMEOUT_BEGIN;
                          CoursesManager courses_manager = CoursesManager();
                          EXPECT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
                          EXPECT_EQ(courses_manager.AddCourse(5, 10), FAILURE);
                          EXPECT_EQ(courses_manager.AddCourse(5, 9), FAILURE);
                          EXPECT_EQ(courses_manager.AddCourse(10, 5), SUCCESS);

                          EXPECT_EQ(courses_manager.AddCourse(6, 0), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.AddCourse(6, -1), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.AddCourse(0, 10), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.AddCourse(-1, 10), INVALID_INPUT);
                          for (int i = 11; i < COUNT; i++) {
                              EXPECT_EQ(courses_manager.AddCourse(i, 2), SUCCESS);
                          }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestCoursesManager, RemoveCoourse) {
    TEST_TIMEOUT_BEGIN;
                          CoursesManager courses_manager = CoursesManager();
                          EXPECT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
                          EXPECT_EQ(courses_manager.RemoveCourse(0), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.RemoveCourse(-1), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.RemoveCourse(1), FAILURE);
                          EXPECT_EQ(courses_manager.RemoveCourse(5), SUCCESS);
                          for (int i = 11; i < COUNT; i++) {
                              EXPECT_EQ(courses_manager.AddCourse(i, 2), SUCCESS);
                              EXPECT_EQ(courses_manager.RemoveCourse(i), SUCCESS);
                          }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestCoursesManager, WatchClass) {
    TEST_TIMEOUT_BEGIN;
                          CoursesManager courses_manager = CoursesManager();
                          EXPECT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
                          EXPECT_EQ(courses_manager.WatchClass(0, 1, 2), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.WatchClass(-1, 1, 2), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.WatchClass(5, -1, 2), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.WatchClass(5, 11, 2), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.WatchClass(2020, 2, 2), FAILURE);

                          EXPECT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
                          EXPECT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
                          EXPECT_EQ(courses_manager.WatchClass(5, 9, 2020), SUCCESS);
                          for (int i = 11; i < COUNT; i++) {
                              EXPECT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
                          }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestCoursesManager, TimeViewed) {
    TEST_TIMEOUT_BEGIN;
                          CoursesManager courses_manager = CoursesManager();
                          EXPECT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
                          EXPECT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
                          int* time(new(int));
                          EXPECT_EQ(courses_manager.TimeViewed(0, 3, time), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.TimeViewed(-1, 3, time), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.TimeViewed(5, -1, time), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.TimeViewed(5, 2020, time), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.TimeViewed(2020, 4, time), FAILURE);
                          EXPECT_EQ(courses_manager.TimeViewed(5, 0, time), SUCCESS);
                          EXPECT_EQ(*time, 1);
                          EXPECT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
                          EXPECT_EQ(courses_manager.TimeViewed(5, 0, time), SUCCESS);
                          EXPECT_EQ(*time, 2);
                          for (int i = 11; i < COUNT; i++) {
                              EXPECT_EQ(courses_manager.TimeViewed(5, 0, time), SUCCESS);
                              EXPECT_EQ(*time, 2);
                          }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}

TEST(TestCoursesManager, GetMostViewedClasses) {
    TEST_TIMEOUT_BEGIN;
                          CoursesManager courses_manager = CoursesManager();
                          EXPECT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
                          int* courses;
                          courses = new int[10];
                          int* class_arr;
                          class_arr = new int[10];
                          EXPECT_EQ(courses_manager.GetMostViewedClasses(0,courses,class_arr), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.GetMostViewedClasses(-1,courses,class_arr), INVALID_INPUT);
                          EXPECT_EQ(courses_manager.GetMostViewedClasses(2020,courses,class_arr), FAILURE);
                          EXPECT_EQ(courses_manager.GetMostViewedClasses(11,courses,class_arr), FAILURE);
                          EXPECT_EQ(courses_manager.GetMostViewedClasses(10,courses,class_arr), SUCCESS);
                          for(int i=0;i<10;i++){
                              EXPECT_EQ(class_arr[i], 10-i);
                              EXPECT_EQ(courses[i], 5);
                          }
                          EXPECT_EQ(courses_manager.AddCourse(6, 10), SUCCESS);
                          EXPECT_EQ(courses_manager.GetMostViewedClasses(20,courses,class_arr), SUCCESS);
                          for(int i=0;i<10;i++){
                              EXPECT_EQ(class_arr[i], 10-i);
                              EXPECT_EQ(courses[i], 5);
                          }
                          for(int i=10;i<20;i++){
                              EXPECT_EQ(class_arr[i], 10-i);
                              EXPECT_EQ(courses[i], 6);
                          }
    TEST_TIMEOUT_FAIL_END(TIMEOUT);
}