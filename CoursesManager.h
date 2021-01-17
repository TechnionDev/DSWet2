#ifndef WET_HW1_COURSESMANAGER_H
#define WET_HW1_COURSESMANAGER_H

#include "BinTree.h"
#include "CourseNode.h"
#include "Key.h"
#include "library2.h"

namespace LecturesStats {

class CoursesManager {
   private:
    BinTree<Key, void*>* views_tree;
    HashMap<shared_ptr<CourseNode>> course_hash_map;

   public:
    CoursesManager();
    StatusType AddCourse(int courseID);
    StatusType RemoveCourse(int courseID);
    StatusType AddClass(int courseID, int* classID);
    StatusType WatchClass(int courseID, int classID, int time);
    StatusType TimeViewed(int courseID, int classID, int* timeViewed);
    StatusType GetIthWatchedClass(int i, int* courseID, int* classID);
    ~CoursesManager();
};

}  // namespace LecturesStats
#endif  // WET_HW1_COURSESMANAGER_H
