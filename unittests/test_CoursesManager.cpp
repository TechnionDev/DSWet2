#include <time.h>

#include <map>
#include <string>
#include <vector>

#include "helperTest.h"

const int CLASSES_COUNT = 200;  // ms
const int SUM_LOG = 8529;       // ms -> sum i=1 to 1000 (log_2(i))

// Helper functions
TEST(TestCoursesManager, InsertCourse) {
    CoursesManager courses_manager = CoursesManager();
    ASSERT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
    ASSERT_EQ(courses_manager.AddCourse(5, 10), FAILURE);
    ASSERT_EQ(courses_manager.AddCourse(5, 9), FAILURE);
    ASSERT_EQ(courses_manager.AddCourse(10, 5), SUCCESS);

    ASSERT_EQ(courses_manager.AddCourse(6, 0), INVALID_INPUT);
    ASSERT_EQ(courses_manager.AddCourse(6, -1), INVALID_INPUT);
    ASSERT_EQ(courses_manager.AddCourse(0, 10), INVALID_INPUT);
    ASSERT_EQ(courses_manager.AddCourse(-1, 10), INVALID_INPUT);
}

TEST(TestCoursesManager, InsertCourseTimer) {
    TEST_TIMEOUT_BEGIN;
    CoursesManager courses_manager = CoursesManager();
    for (int i = 1; i < COUNT; i++) {
        ASSERT_EQ(courses_manager.AddCourse(i, CLASSES_COUNT), SUCCESS);
    }

    //    ASSERT_EQ(courses_manager.AddCourse(1, CLASSES_COUNT), SUCCESS);
    //    //->(LOG(1)+CLASSES_COUNT)->1 ms time
    TEST_TIMEOUT_FAIL_END((CLASSES_COUNT * COUNT + COUNT * (LOG(COUNT) - 1)) *
                          TIME_UNIT);
}

TEST(TestCoursesManager, RemoveCourse) {
    CoursesManager courses_manager = CoursesManager();
    ASSERT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
    ASSERT_EQ(courses_manager.RemoveCourse(0), INVALID_INPUT);
    ASSERT_EQ(courses_manager.RemoveCourse(-1), INVALID_INPUT);
    ASSERT_EQ(courses_manager.RemoveCourse(1), FAILURE);

    ASSERT_EQ(courses_manager.AddCourse(6, 20), SUCCESS);
    ASSERT_EQ(courses_manager.RemoveCourse(5), SUCCESS);
    ASSERT_EQ(courses_manager.RemoveCourse(6), SUCCESS);
    ASSERT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
    ASSERT_EQ(courses_manager.AddCourse(6, 20), SUCCESS);
    ASSERT_EQ(courses_manager.WatchClass(6, 4, 2020), SUCCESS);
    ASSERT_EQ(courses_manager.WatchClass(5, 3, 2020), SUCCESS);
    int courses[1] = {0};
    int class_arr[1] = {0};
    ASSERT_EQ(courses_manager.GetMostViewedClasses(1, courses, class_arr),
              SUCCESS);
    ASSERT_EQ(courses[0], 5);
    ASSERT_EQ(class_arr[0], 3);
    ASSERT_EQ(courses_manager.RemoveCourse(5), SUCCESS);
    ASSERT_EQ(courses_manager.RemoveCourse(6), SUCCESS);
    ASSERT_EQ(courses_manager.WatchClass(6, 4, 2020), FAILURE);
    ASSERT_EQ(courses_manager.WatchClass(5, 3, 2020), FAILURE);

    ASSERT_EQ(courses_manager.AddCourse(1, 100), SUCCESS);
    ASSERT_EQ(courses_manager.GetMostViewedClasses(1, courses, class_arr),
              SUCCESS);
    ASSERT_EQ(courses[0], 1);
    ASSERT_EQ(class_arr[0], 0);
}

TEST(TestCoursesManager, RemoveCourseTimer_1) {
    TEST_TIMEOUT_BEGIN;
    CoursesManager courses_manager = CoursesManager();
    for (int i = 1; i < COUNT; i++) {
        ASSERT_EQ(courses_manager.AddCourse(i, CLASSES_COUNT), SUCCESS);
        ASSERT_EQ(courses_manager.RemoveCourse(i), SUCCESS);
    }
    //                          ASSERT_EQ(courses_manager.AddCourse(1,
    //                          CLASSES_COUNT), SUCCESS);
    //                          ASSERT_EQ(courses_manager.RemoveCourse(1),
    //                          SUCCESS);//both of them together were 2~1 ms
    TEST_TIMEOUT_FAIL_END(2 * CLASSES_COUNT * COUNT * TIME_UNIT);
}

TEST(TestCoursesManager, RemoveCourseTimer_2) {
    TEST_TIMEOUT_BEGIN;
    CoursesManager courses_manager = CoursesManager();
    for (int i = 1; i < COUNT; i++) {
        ASSERT_EQ(courses_manager.AddCourse(i, CLASSES_COUNT), SUCCESS);
    }
    for (int i = 1; i < COUNT; i++) {
        ASSERT_EQ(courses_manager.RemoveCourse(i), SUCCESS);
    }

    TEST_TIMEOUT_FAIL_END(((CLASSES_COUNT * COUNT + COUNT * (LOG(COUNT) - 1)) +
                           CLASSES_COUNT * (CLASSES_COUNT * COUNT *
                                            (LOG(CLASSES_COUNT * COUNT) - 1))) *
                          TIME_UNIT);
}

TEST(TestCoursesManager, AllLecturesOneCourse) {
    TEST_TIMEOUT_BEGIN;
    CoursesManager courses_manager = CoursesManager();
    ASSERT_EQ(courses_manager.AddCourse(1, COUNT), SUCCESS);

    ASSERT_EQ(courses_manager.RemoveCourse(1), SUCCESS);

    TEST_TIMEOUT_FAIL_END((COUNT + COUNT * LOG(COUNT)) * TIME_UNIT);
}

TEST(TestCoursesManager, WatchClass) {
    CoursesManager courses_manager = CoursesManager();
    ASSERT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
    ASSERT_EQ(courses_manager.WatchClass(0, 1, 2), INVALID_INPUT);
    ASSERT_EQ(courses_manager.WatchClass(-1, 1, 2), INVALID_INPUT);
    ASSERT_EQ(courses_manager.WatchClass(5, -1, 2), INVALID_INPUT);
    ASSERT_EQ(courses_manager.WatchClass(5, 11, 2), INVALID_INPUT);
    ASSERT_EQ(courses_manager.WatchClass(2020, 2, 2), FAILURE);

    ASSERT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
    ASSERT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
    ASSERT_EQ(courses_manager.WatchClass(5, 9, 2020), SUCCESS);
}

TEST(TestCoursesManager, WatchClassTimer) {
    TEST_TIMEOUT_BEGIN;
    CoursesManager courses_manager = CoursesManager();
    for (int i = 1; i < COUNT; i++) {
        ASSERT_EQ(courses_manager.AddCourse(i, CLASSES_COUNT), SUCCESS);
    }
    for (int i = 1; i < COUNT; i++) {
        ASSERT_EQ(courses_manager.WatchClass(i, 1, i), SUCCESS);
    }
    TEST_TIMEOUT_FAIL_END(((10 + COUNT) + 500500 + 27) * TIME_UNIT);  //???
}
/**
 * log(M)+t
 * i: log(CLASSES_COUNT*(count))*COUNT +500500(->sum (j) j=0 to count)
 */
TEST(TestCoursesManager, TimeViewed) {
    TEST_TIMEOUT_BEGIN;
    CoursesManager courses_manager = CoursesManager();
    int time;
    ASSERT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
    ASSERT_EQ(courses_manager.TimeViewed(5, 0, &time), SUCCESS);
    ASSERT_EQ(time, 0);
    ASSERT_EQ(courses_manager.TimeViewed(5, 9, &time), SUCCESS);
    ASSERT_EQ(time, 0);

    ASSERT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
    ASSERT_EQ(courses_manager.TimeViewed(0, 3, &time), INVALID_INPUT);
    ASSERT_EQ(courses_manager.TimeViewed(-1, 3, &time), INVALID_INPUT);
    ASSERT_EQ(courses_manager.TimeViewed(5, -1, &time), INVALID_INPUT);
    ASSERT_EQ(courses_manager.TimeViewed(5, 2020, &time), INVALID_INPUT);
    ASSERT_EQ(courses_manager.TimeViewed(2020, 4, &time), FAILURE);
    ASSERT_EQ(courses_manager.TimeViewed(5, 0, &time), SUCCESS);
    ASSERT_EQ(time, 1);
    ASSERT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
    ASSERT_EQ(courses_manager.TimeViewed(5, 0, &time), SUCCESS);
    ASSERT_EQ(time, 2);
    ASSERT_EQ(courses_manager.TimeViewed(5, 0, &time), SUCCESS);
    ASSERT_EQ(time, 2);

    TEST_TIMEOUT_FAIL_END(150 * TIME_UNIT);
}

TEST(TestCoursesManager, GetMostViewedClasses) {
    TEST_TIMEOUT_BEGIN;
    CoursesManager courses_manager = CoursesManager();
    ASSERT_EQ(courses_manager.AddCourse(5, 10), SUCCESS);
    int courses[10] = {0};
    int class_arr[10] = {0};
    ASSERT_EQ(courses_manager.GetMostViewedClasses(0, courses, class_arr),
              INVALID_INPUT);
    ASSERT_EQ(courses_manager.GetMostViewedClasses(-1, courses, class_arr),
              INVALID_INPUT);
    ASSERT_EQ(courses_manager.GetMostViewedClasses(2020, courses, class_arr),
              FAILURE);
    ASSERT_EQ(courses_manager.GetMostViewedClasses(11, courses, class_arr),
              FAILURE);
    ASSERT_EQ(courses_manager.GetMostViewedClasses(10, courses, class_arr),
              SUCCESS);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(class_arr[i], i);
        ASSERT_EQ(courses[i], 5);
    }
    ASSERT_EQ(courses_manager.AddCourse(6, 10), SUCCESS);
    int courses_2[20] = {0};
    int class_arr_2[20] = {0};
    ASSERT_EQ(courses_manager.GetMostViewedClasses(20, courses_2, class_arr_2),
              SUCCESS);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(class_arr_2[i], i);
        ASSERT_EQ(courses_2[i], 5);
    }
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(class_arr_2[i+10], i);
        ASSERT_EQ(courses_2[i+10], 6);
    }
    ASSERT_EQ(courses_manager.WatchClass(5, 4, 1000), SUCCESS);
    ASSERT_EQ(courses_manager.GetMostViewedClasses(10, courses, class_arr),
              SUCCESS);
    ASSERT_EQ(class_arr[0], 4);
    ASSERT_EQ(courses[0], 5);
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(class_arr[i+1], i);
        ASSERT_EQ(courses[i+1], 5);
    }
    for (int i = 5; i < 10; i++) {
        ASSERT_EQ(class_arr[i], i);
        ASSERT_EQ(courses[i], 5);
    }
    int courses_3[2020] = {0};
    int class_arr_3[2020] = {0};
    ASSERT_EQ(
        courses_manager.GetMostViewedClasses(2020, courses_3, class_arr_3),
        FAILURE);
    TEST_TIMEOUT_FAIL_END(150 * TIME_UNIT);
}
