
#include "CoursesManager.h"

namespace LecturesStats {
    StatusType CoursesManager::GetIthWatchedClass(int i, int* courseID, int* classID) {
        try {
            if (i <= 0) {
                return INVALID_INPUT;
            }
            if (i > views_tree->sizeOfTree()) {
                return FAILURE;
            }
            auto ith_class = views_tree->getN(views_tree->sizeOfTree() - i + 1);
            *courseID = ith_class.course_id;
            *classID = ith_class.lecture_id;
            return SUCCESS;
        } catch (...) {
            return ALLOCATION_ERROR;
        }
    }

    StatusType CoursesManager::TimeViewed(int courseID, int classID, int* timeViewed) {
        if (courseID <= 0 || classID < 0) {
            return INVALID_INPUT;
        }
        try {
            if (not course_hash_map.exist(courseID)) {
                return FAILURE;
            }
            shared_ptr<CourseNode> course_hash_map_ptr = course_hash_map.get(courseID);
            if (classID + 1 > course_hash_map_ptr->get_class_num()) {
                return INVALID_INPUT;
            }
            *timeViewed = course_hash_map_ptr->get_class(classID)->get_views();
        } catch (...) {
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    StatusType CoursesManager::WatchClass(int courseID, int classID, int time) {
        if (courseID <= 0 || classID < 0 || time <= 0) {
            return INVALID_INPUT;
        }
        try {
            if (not course_hash_map.exist(courseID)) {
                return FAILURE;
            }
            shared_ptr<CourseNode> course_hash_map_ptr = course_hash_map.get(courseID);
            if (classID + 1 > course_hash_map_ptr->get_class_num()) {
                return INVALID_INPUT;
            }
            //update the views in the lecture
            int time_viewed_old = course_hash_map_ptr->get_class(classID)->get_views();
            course_hash_map_ptr->get_class(classID)->add_views(time);
            //update the location of the lecture in the tree
            if (time_viewed_old != 0) {
                //if the lecture is already in the view_tree
                views_tree->pop(Key(time_viewed_old, courseID, classID));
                views_tree->add(Key(time_viewed_old + time, courseID, classID), nullptr);
            } else {
                views_tree->add(Key(time_viewed_old + time, courseID, classID), nullptr);
            }
        } catch (...) {
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    StatusType CoursesManager::RemoveCourse(int courseID) {
        if (courseID <= 0) {
            return INVALID_INPUT;
        }
        try {
            if (not course_hash_map.exist(courseID)) {
                return FAILURE;
            }
            shared_ptr<CourseNode> course_ptr = course_hash_map.get(courseID);
            for (int i = 0; i < course_ptr->get_class_num(); i++) {
                if (course_ptr->get_class(i)->get_views() != 0) {
                    views_tree->pop(Key(course_ptr->get_class(i)->get_views(), courseID, i));
                }
            }
            course_ptr->pop_lectures();
            course_hash_map.remove(courseID);
        } catch (...) {
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    StatusType CoursesManager::AddCourse(int courseID) {
        if (courseID <= 0) {
            return INVALID_INPUT;
        }
        if (course_hash_map.exist(courseID)) {
            return FAILURE;
        }
        shared_ptr<CourseNode> course_ptr(new CourseNode(courseID));
        try {
            course_hash_map.set(courseID, course_ptr);
        } catch (...) {
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    StatusType CoursesManager::AddClass(int courseID, int* classID) {
        if (courseID <= 0) {
            return INVALID_INPUT;
        }
        if (not course_hash_map.exist(courseID)) {
            return FAILURE;
        }
        *classID = course_hash_map.get(courseID)->insert_class();
        return SUCCESS;
    }

    CoursesManager::CoursesManager()
            : views_tree(new BinTree<Key, void*>()) {
    }

    CoursesManager::~CoursesManager() {
        delete (views_tree);
    }

}  // namespace LecturesStats