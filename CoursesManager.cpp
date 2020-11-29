
#include "CoursesManager.h"

namespace LecturesStats {
    StatusType CoursesManager::GetMostViewedClasses(int numOfClasses, int* courses, int* classes) {
        if (numOfClasses <= 0) {
            return INVALID_INPUT;
        }
        int index = 0;
        ListNode* current_node = top_node;
        while (numOfClasses > 0) {
            for (auto it_course = current_node->get_begin_iterator();
                 it_course != current_node->get_end_iterator(); it_course++) {
                for (auto it_lecture = (!it_course).begin();
                     it_lecture != (!it_course).end(); it_lecture++) {//todo::change to it.key/value
                    if (numOfClasses > 0) {
                        courses[index] = *it_course;
                        classes[index] = *it_lecture;
                        numOfClasses--;
                        index++;
                    }
                }
            }
            if (current_node->get_prev_node() == nullptr) {
                return FAILURE;
            }
            current_node = current_node->get_prev_node();
        }
        return SUCCESS;
    }

    StatusType CoursesManager::TimeViewed(int courseID, int classID, int* timeViewed) {
        if (courseID <= 0 || classID < 0) {
            return INVALID_INPUT;
        }
        int views = 0;
        try {
            shared_ptr<CourseNode> course_tree_ptr = course_tree.get(courseID);
            if (course_tree_ptr == nullptr) {
                return FAILURE;
            }
            if (classID + 1 > course_tree_ptr->get_class_num()) {
                return INVALID_INPUT;
            }
            shared_ptr<Lecture> lecture = course_tree_ptr->get_class(classID);
            views = lecture->get_views();
        } catch (...) { return ALLOCATION_ERROR; }
        *timeViewed = views;
        return SUCCESS;
    }

    StatusType CoursesManager::WatchClass(int courseID, int classID, int time) {
        if (courseID <= 0 || classID < 0 || time <= 0) {
            return INVALID_INPUT;
        }
        try {
            shared_ptr<CourseNode> course_tree_ptr = course_tree.get(courseID);
            if (course_tree_ptr == nullptr) {
                return FAILURE;
            }
            if (classID + 1 > course_tree_ptr->get_class_num()) {
                return INVALID_INPUT;
            }
            shared_ptr<Lecture> lecture = course_tree_ptr->get_class(classID);
            lecture->add_views(time, top_node);
        } catch (...) { return ALLOCATION_ERROR; }
        return SUCCESS;
    }

    StatusType CoursesManager::RemoveCourse(int courseID) {
        if (courseID <= 0) {
            return INVALID_INPUT;
        }
        shared_ptr<CourseNode> course_tree_ptr = course_tree.get(courseID);
        if (course_tree_ptr == nullptr) {
            return FAILURE;
        }
        try {
            course_tree_ptr->remove();
            course_tree.pop(courseID);
        } catch (...) { return ALLOCATION_ERROR; }
        return SUCCESS;
    }

    StatusType CoursesManager::AddCourse(int courseID, int numOfClasses) {
        if (courseID <= 0 || numOfClasses <= 0) {
            return INVALID_INPUT;
        }
        if (course_tree.get(courseID) == nullptr) {
            shared_ptr<CourseNode> course_ptr(new CourseNode(numOfClasses, courseID, bottom_node));
            try {
                course_tree.add(courseID, course_ptr);
            } catch (...) { return ALLOCATION_ERROR; }
        } else {
            return FAILURE;
        }
        return SUCCESS;
    }

    CoursesManager::CoursesManager() : bottom_node(new ListNode(0)),
                                       course_tree(new LecturesStats::BinTree<int, CourseNode>)//todo::dont get it {
    }

    void CoursesManager::Quit() {
        for (auto it_course = top_node->get_begin_iterator();
             it_course != top_node->get_end_iterator(); it_course++) {
            for (auto it_lecture = (!it_course).begin();
                 it_lecture != (!it_course).end(); it_lecture++) {//todo::change to it.key/value
                //todo::need to check with gut if the iterator can remove vertex by itself
            }
        }
    }
}