
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
            auto course_tree = views_tree->getN(views_tree->sizeOfTree() - i+1);
            assert(course_tree!= nullptr);
            auto lecture_tree = course_tree->getMax();
            assert(lecture_tree!= nullptr);
            *courseID = course_tree->getMaxKey();
            *classID = lecture_tree->getMaxKey();
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

    void CoursesManager::remove_lecture_view_tree(int courseID, int classID, int time_viwed) {
        auto course_lecture_tree = views_tree->get(time_viwed);
        auto lecture_node = course_lecture_tree->get(courseID);
        lecture_node->pop(classID);
        if (lecture_node->isEmpty()) {
            course_lecture_tree->pop(courseID);
        }
        if (course_lecture_tree->isEmpty()) {
            views_tree->pop(time_viwed);
        }
    }

    void CoursesManager::add_lecture_view_tree(shared_ptr<BinTree<int, BinTree<int, void*>>> course_lecture_tree,
                                               int courseID, int classID, int time) {
        if (course_lecture_tree == nullptr) {
            //if our lecture is the only one to have that num of views
            shared_ptr<BinTree<int, BinTree<int, void* >>> course_tree_ptr(
                    new BinTree<int, BinTree<int, void*>>);
            shared_ptr<BinTree<int, void*>> lecture_tree_ptr(
                    new BinTree<int, void*>);
            views_tree->add(time, course_tree_ptr);
            course_tree_ptr->add(courseID, lecture_tree_ptr);
            lecture_tree_ptr->add(classID, nullptr);
        } else {
            //if there is a lecture with the new num of views in the view_tree
            auto course_node = course_lecture_tree->get(courseID);
            if (course_node == nullptr) {
                shared_ptr<BinTree<int, void*>> lecture_tree_ptr(
                        new BinTree<int, void*>);
                course_lecture_tree->add(courseID, lecture_tree_ptr);
                lecture_tree_ptr->add(classID, nullptr);
            } else {
                course_node->add(classID, nullptr);
            }
        }
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
            int time_viwed_old = course_hash_map_ptr->get_class(classID)->get_views();
            course_hash_map_ptr->get_class(classID)->add_views(time);
            //update the location of the lecture in the tree
            if (time_viwed_old != 0) {
                //if the lecture is already in the view_tree
                remove_lecture_view_tree(courseID, classID, time_viwed_old);
                auto course_lecture_tree = views_tree->get(time_viwed_old + time);
                add_lecture_view_tree(course_lecture_tree, courseID, classID, time_viwed_old + time);
            } else {
                auto course_lecture_tree = views_tree->get(time_viwed_old + time);
                add_lecture_view_tree(course_lecture_tree, courseID, classID, time_viwed_old + time);
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
                remove_lecture_view_tree(courseID, i, course_ptr->get_class(i)->get_views());
            }
            course_ptr->pop_lectures();
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
            : views_tree(new BinTree<int, BinTree<int, BinTree<int, void*>>>()) {
    }

    CoursesManager::~CoursesManager() {
        delete (views_tree);
    }

}  // namespace LecturesStats