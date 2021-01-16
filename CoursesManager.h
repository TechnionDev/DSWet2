#ifndef WET_HW1_COURSESMANAGER_H
#define WET_HW1_COURSESMANAGER_H

#include "CourseNode.h"
#include "library2.h"
#include "BinTree.h"

namespace LecturesStats {

    class CoursesManager {
    private:
        BinTree<int, BinTree<int, BinTree<int, void*>>>* views_tree;
        HashMap<shared_ptr<CourseNode>> course_hash_map ;
        void remove_lecture_view_tree(int courseID, int classID, int time_viwed);
        void add_lecture_view_tree(shared_ptr<BinTree<int, BinTree<int, void*>>> course_lecture_tree,
                                   int courseID, int classID, int time_viwed);
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
