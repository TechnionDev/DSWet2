#include <time.h>
#include <map>
#include <string>
#include <vector>
#include "helperTest.h"

#define  REALY_BIG_NUM 100
TEST(TestCoursesManager, InsertCourse) {
    CoursesManager courses_manager = CoursesManager();
    ASSERT_EQ(courses_manager.AddCourse(5), SUCCESS);
    ASSERT_EQ(courses_manager.AddCourse(5), FAILURE);
    ASSERT_EQ(courses_manager.AddCourse(6), SUCCESS);
    ASSERT_EQ(courses_manager.AddCourse(0), INVALID_INPUT);
    ASSERT_EQ(courses_manager.AddCourse(-1), INVALID_INPUT);
}

TEST(TestCoursesManager, InsertClass) {
    CoursesManager courses_manager = CoursesManager();
    ASSERT_EQ(courses_manager.AddCourse(5), SUCCESS);
    ASSERT_EQ(courses_manager.AddCourse(6), SUCCESS);
    ASSERT_EQ(courses_manager.AddCourse(1), SUCCESS);
    //check for invalid_inputs
    int class_id;

    ASSERT_EQ(courses_manager.AddClass(-1, &class_id), INVALID_INPUT);
    ASSERT_EQ(courses_manager.AddClass(0, &class_id), INVALID_INPUT);
    ASSERT_EQ(courses_manager.AddClass(666, &class_id), FAILURE);

    ASSERT_EQ(courses_manager.AddClass(5, &class_id), SUCCESS);
    ASSERT_EQ(class_id, 0);
    ASSERT_EQ(courses_manager.AddClass(5, &class_id), SUCCESS);
    ASSERT_EQ(class_id, 1);
    ASSERT_EQ(courses_manager.AddClass(5, &class_id), SUCCESS);
    ASSERT_EQ(class_id, 2);
    ASSERT_EQ(courses_manager.AddClass(1, &class_id), SUCCESS);
    ASSERT_EQ(class_id, 0);
    ASSERT_EQ(courses_manager.AddClass(1, &class_id), SUCCESS);
    ASSERT_EQ(class_id, 1);

    ASSERT_EQ(courses_manager.AddCourse(666), SUCCESS);
    for (int i = 0; i < REALY_BIG_NUM; i++) {
        ASSERT_EQ(courses_manager.AddClass(666, &class_id), SUCCESS);
        ASSERT_EQ(class_id, i);
    }

}

TEST(TestCoursesManager, WatchClass) {
    CoursesManager courses_manager = CoursesManager();
    ASSERT_EQ(courses_manager.AddCourse(5), SUCCESS);
    ASSERT_EQ(courses_manager.AddCourse(6), SUCCESS);
    int class_id;
    ASSERT_EQ(courses_manager.AddClass(5, &class_id), SUCCESS);
    ASSERT_EQ(courses_manager.AddClass(6, &class_id), SUCCESS);

    ASSERT_EQ(courses_manager.WatchClass(5, 0, -1), INVALID_INPUT);
    ASSERT_EQ(courses_manager.WatchClass(5, -1, 2), INVALID_INPUT);
    ASSERT_EQ(courses_manager.WatchClass(-1, 0, 2), INVALID_INPUT);
    ASSERT_EQ(courses_manager.WatchClass(5, 1, 2), INVALID_INPUT);
    ASSERT_EQ(courses_manager.WatchClass(2020, 0, 2), FAILURE);
    int time;
    ASSERT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
    ASSERT_EQ(courses_manager.TimeViewed(5, 0, &time), SUCCESS);
    ASSERT_EQ(time, 1);
    ASSERT_EQ(courses_manager.WatchClass(5, 0, 1), SUCCESS);
    ASSERT_EQ(courses_manager.TimeViewed(5, 0, &time), SUCCESS);
    ASSERT_EQ(time, 2);
    ASSERT_EQ(courses_manager.WatchClass(5, 0, 2021), SUCCESS);
    ASSERT_EQ(courses_manager.TimeViewed(5, 0, &time), SUCCESS);
    ASSERT_EQ(time, 2023);

    ASSERT_EQ(courses_manager.WatchClass(6, 0, 2021), SUCCESS);
    ASSERT_EQ(courses_manager.TimeViewed(6, 0, &time), SUCCESS);
    ASSERT_EQ(time, 2021);

    ASSERT_EQ(courses_manager.AddCourse(1), SUCCESS);
    ASSERT_EQ(courses_manager.AddClass(5, &class_id), SUCCESS);
    for (int course = 10; course < REALY_BIG_NUM; course++) {
        ASSERT_EQ(courses_manager.AddCourse(course), SUCCESS);
        for (int lecture = 0; lecture < REALY_BIG_NUM; lecture++) {
            int time_counter = 0;
            int lecture_id;
            ASSERT_EQ(courses_manager.AddClass(course, &lecture_id), SUCCESS);
            ASSERT_EQ(lecture_id, lecture);
            for (int views = 1; views < REALY_BIG_NUM; views++) {
                time_counter += views;
                ASSERT_EQ(courses_manager.WatchClass(course, lecture, views), SUCCESS);
                ASSERT_EQ(courses_manager.TimeViewed(course, lecture, &time), SUCCESS);
                ASSERT_EQ(time, time_counter);
            }
        }
    }
}

TEST(TestCoursesManager, TimeViewed) {
    CoursesManager courses_manager = CoursesManager();
    int time, class_id;
    ASSERT_EQ(courses_manager.AddCourse(5), SUCCESS);
    ASSERT_EQ(courses_manager.AddClass(5, &class_id), SUCCESS);
    ASSERT_EQ(courses_manager.TimeViewed(5, -1, &time), INVALID_INPUT);
    ASSERT_EQ(courses_manager.TimeViewed(0, 1, &time), INVALID_INPUT);
    ASSERT_EQ(courses_manager.TimeViewed(-1, 1, &time), INVALID_INPUT);
    ASSERT_EQ(courses_manager.TimeViewed(5, 4, &time), INVALID_INPUT);
    ASSERT_EQ(courses_manager.TimeViewed(4, 0, &time), FAILURE);
    ASSERT_EQ(courses_manager.TimeViewed(5, 0, &time), SUCCESS);
    ASSERT_EQ(time, 0);
}

TEST(TestCoursesManager, GetIthMostViewed_small) {
    CoursesManager courses_manager = CoursesManager();
    int class_id;
    for (int i = 1; i <= 10; i++) {
        ASSERT_EQ(courses_manager.AddCourse(i), SUCCESS);
        ASSERT_EQ(courses_manager.AddClass(i, &class_id), SUCCESS);
        ASSERT_EQ(class_id, 0);
        ASSERT_EQ(courses_manager.WatchClass(i, 0, i), SUCCESS);
    }
    int course_id, lecture_id;
    ASSERT_EQ(courses_manager.GetIthWatchedClass(1, &course_id, &lecture_id), SUCCESS);
    ASSERT_EQ(course_id, 10);
    ASSERT_EQ(lecture_id, 0);
    ASSERT_EQ(courses_manager.GetIthWatchedClass(10, &course_id, &lecture_id), SUCCESS);
    ASSERT_EQ(course_id, 1);
    ASSERT_EQ(lecture_id, 0);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(courses_manager.GetIthWatchedClass(10 - i, &course_id, &lecture_id), SUCCESS);
        ASSERT_EQ(course_id, 1 + i);
        ASSERT_EQ(lecture_id, 0);
    }
}

TEST(TestCoursesManager, GetIthMostViewed) {
    CoursesManager courses_manager = CoursesManager();
    int time = 1, class_id;
    for (int course_id = 1; course_id <= REALY_BIG_NUM; course_id++) {
        ASSERT_EQ(courses_manager.AddCourse(course_id), SUCCESS);
        for (int lecture_id = 0; lecture_id <= REALY_BIG_NUM; lecture_id++) {
            ASSERT_EQ(courses_manager.AddClass(course_id, &class_id), SUCCESS);
            ASSERT_EQ(class_id, lecture_id);
            ASSERT_EQ(courses_manager.WatchClass(course_id, lecture_id, time), SUCCESS);
            time++;
        }
    }
    int course_id, lecture_id;
    ASSERT_EQ(courses_manager.GetIthWatchedClass(1, &course_id, &lecture_id), SUCCESS);
    ASSERT_EQ(course_id, REALY_BIG_NUM);
    ASSERT_EQ(lecture_id, REALY_BIG_NUM);
    ASSERT_EQ(courses_manager.GetIthWatchedClass(REALY_BIG_NUM, &course_id, &lecture_id), SUCCESS);
    ASSERT_EQ(course_id, 100);
    ASSERT_EQ(lecture_id, 1);

}