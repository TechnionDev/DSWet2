#ifndef WET_HW1_COURSESMANAGER_H
#define WET_HW1_COURSESMANAGER_H

#include "CourseNode.h"

namespace LecturesStats {
    typedef enum {
        SUCCESS = 0,
        FAILURE = -1,
        ALLOCATION_ERROR = -2,
        INVALID_INPUT = -3
    } StatusType;

    class CoursesManager {
    private:
        LecturesStats::BinTree<int, CourseNode> course_tree;
        ListNode* bottom_node;
        ListNode* top_node = bottom_node;
    public:
        CoursesManager();
        StatusType AddCourse(int courseID, int numOfClasses);
        StatusType RemoveCourse(int courseID);
        StatusType WatchClass(int courseID, int classID, int time);
        StatusType TimeViewed(int courseID, int classID, int* timeViewed);
        StatusType GetMostViewedClasses(int numOfClasses, int* courses, int* classes);
        void Quit();
    };

}
#endif //WET_HW1_COURSESMANAGER_H
