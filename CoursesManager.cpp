
#include "CoursesManager.h"

StatusType CoursesManager::GetMostViewedClasses(int numOfClasses, int* courses, int* classes) {
    if (numOfClasses <= 0) {
        return INVALID_INPUT;
    }
    int i = 0;
    while (numOfClasses > 0) {
        for (BinTree<int, shared_ptr<BinTree<int, shared_ptr<Lecture>>>>::iterator it = top_node->get_begin_iterator();
             it != top_node->get_end_iterator(); it++) {
            for (BinTree<int, shared_ptr<Lecture>>::iterator it_lecture = it->get_begin_iterator();
                 it_lecture != it->get_end_iterator(); it_lecture++){
                //todo::complete the function when the iterator is done
            }
        }
    }
    return SUCCESS;
}

StatusType CoursesManager::TimeViewed(int courseID, int classID, int* timeViewed) {
    if (courseID <= 0 || classID < 0) {
        return INVALID_INPUT;
    }
    int viwes;
    try {
        shared_ptr<CourseNode> course_tree_ptr = coures_tree.get(courseID);
        if (classID + 1 > course_tree_ptr->get_class_num()) {
            return INVALID_INPUT;
        }
        shared_ptr<Lecture> lecture = course_tree_ptr->get_class(classID);
        if (lecture == nullptr) {
            return FAILURE;
        }
        viwes = lecture->get_views();
    } catch () {}
    *timeViewed = viwes;
    return SUCCESS;
}

StatusType CoursesManager::WatchClass(int courseID, int classID, int time) {
    if (courseID <= 0 || classID < 0 || time <= 0) {
        return INVALID_INPUT;
    }
    try {
        shared_ptr<CourseNode> course_tree_ptr = coures_tree.get(courseID);
        if (classID + 1 > course_tree_ptr->get_class_num()) {
            return INVALID_INPUT;
        }
        shared_ptr<Lecture> lecture = course_tree_ptr->get_class(classID);
        lecture->add_views(time, top_node);
    } catch () {}//todo::handel exception
    return SUCCESS;
}

StatusType CoursesManager::RemoveCourse(int courseID) {
    if (courseID <= 0) {
        return INVALID_INPUT;
    }
    shared_ptr<CourseNode> course_tree_ptr = coures_tree.get(courseID);
    if (course_tree_ptr == nullptr) {
        return FAILURE;
    }
    try {
        course_tree_ptr->remove();
        coures_tree.pop(courseID);
    } catch () {}//todo::catch ERROR_ALLOCATION ?
    return SUCCESS;
}

StatusType CoursesManager::AddCourse(int courseID, int numOfClasses) {
    if (courseID <= 0 || numOfClasses <= 0) {
        return INVALID_INPUT;
    }
    if (coures_tree.get(courseID) == nullptr) {
        shared_ptr<CourseNode> course_ptr(new CourseNode(numOfClasses, courseID, bottom_node));
        try {
            coures_tree.add(courseID, course_ptr);
            for (int i = 0; i < numOfClasses; i++) {//todo::change to 2 for loops in a way that doesnt require flips
                *bottom_node->insert(course_ptr->get_class(i));
            }
        } catch () {}//todo::catch ERROR_ALLOCATION
    } else {
        return FAILURE;
    }
    return SUCCESS;
}

CoursesManager::CoursesManager() {

}
