#ifndef WET_HW1_COURSESMANAGER_H
#define WET_HW1_COURSESMANAGER_H

#include "CourseNode.h"
#include "library.h"

namespace LecturesStats {

class CoursesManager {
   private:
    BinTree<int, CourseNode>* course_tree;
    ListNode* head;
    ListNode* tail;

   public:
    CoursesManager();
    StatusType AddCourse(int courseID, int numOfClasses);
    StatusType RemoveCourse(int courseID);
    StatusType WatchClass(int courseID, int classID, int time);
    StatusType TimeViewed(int courseID, int classID, int* timeViewed);
    StatusType GetMostViewedClasses(int numOfClasses, int* courses,
                                    int* classes);
    ~CoursesManager();
};

}  // namespace LecturesStats
#endif  // WET_HW1_COURSESMANAGER_H
