
#include "CoursesManager.h"

namespace LecturesStats {
    StatusType CoursesManager::GetMostViewedClasses(int numOfClasses, int* courses,
                                                    int* classes) {
        try {
            if (numOfClasses <= 0) {
                return INVALID_INPUT;
            }
            int index = 0;
            ListNode* current_node = tail;
            while (numOfClasses > 0) {
                for (auto it_course = current_node->get_begin_iterator();
                     it_course != current_node->get_end_iterator(); it_course++) {
                    for (auto it_lecture = it_course.value()->begin();
                         it_lecture != it_course.value()->end(); it_lecture++) {
                        if (numOfClasses > 0) {
                            courses[index] = it_course.key();
                            classes[index] = it_lecture.key();
                            numOfClasses--;
                            index++;
                        } else {
                            break;
                        }
                    }
                    if (numOfClasses == 0) {
                        break;
                    }
                }
                if (numOfClasses > 0) {
                    if (current_node->get_prev_node() == nullptr) {
                        return FAILURE;
                    }
                    current_node = current_node->get_prev_node();
                }
            }
            return SUCCESS;
        } catch (...) {
            return ALLOCATION_ERROR;
        }
    }

    StatusType CoursesManager::TimeViewed(int courseID, int classID,
                                          int* timeViewed) {
        if (courseID <= 0 || classID < 0) {
            return INVALID_INPUT;
        }
        int views = 0;
        try {
            shared_ptr<CourseNode> course_tree_ptr = course_tree->get(courseID);
            if (course_tree_ptr == nullptr) {
                return FAILURE;
            }
            if (classID + 1 > course_tree_ptr->get_class_num()) {
                return INVALID_INPUT;
            }
            shared_ptr<Lecture> lecture = course_tree_ptr->get_class(classID);
            views = lecture->get_views();
        } catch (...) {
            return ALLOCATION_ERROR;
        }
        *timeViewed = views;
        return SUCCESS;
    }

    StatusType CoursesManager::WatchClass(int courseID, int classID, int time) {
        if (courseID <= 0 || classID < 0 || time <= 0) {
            return INVALID_INPUT;
        }
        try {
            shared_ptr<CourseNode> course_tree_ptr = course_tree->get(courseID);
            if (course_tree_ptr == nullptr) {
                return FAILURE;
            }
            if (classID + 1 > course_tree_ptr->get_class_num()) {
                return INVALID_INPUT;
            }
            shared_ptr<Lecture> lecture = course_tree_ptr->get_class(classID);
            lecture->add_views(time, tail);
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
            shared_ptr<CourseNode> course_node_ptr = course_tree->pop(courseID);
            if (course_node_ptr == nullptr) {
                return FAILURE;
            }
            course_node_ptr->pop_lectures(tail);
        } catch (...) {
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    StatusType CoursesManager::AddCourse(int courseID, int numOfClasses) {
        if (courseID <= 0 || numOfClasses <= 0) {
            return INVALID_INPUT;
        }
        if (course_tree->get(courseID) == nullptr) {
            shared_ptr<CourseNode> course_ptr(
                    new CourseNode(numOfClasses, courseID, head));
            try {
                course_tree->add(courseID, course_ptr);
            } catch (...) {
                return ALLOCATION_ERROR;
            }
        } else {
            return FAILURE;
        }
        return SUCCESS;
    }

    CoursesManager::CoursesManager()
            : course_tree(new LecturesStats::BinTree<int, CourseNode>()),
              head(new ListNode(0)) {
        tail = head;
    }

    CoursesManager::~CoursesManager() {
        auto curr = head;

        while (curr != nullptr) {
            auto prev_node = curr;
            curr = curr->get_next_node();
            delete prev_node;
        }
        delete course_tree;
    }
}  // namespace LecturesStats