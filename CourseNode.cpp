#include "CourseNode.h"

namespace LecturesStats {
CourseNode::CourseNode(int numOfClasses, int course_id, ListNode* bottom_node)
    : course_id(course_id), numOfClasses(numOfClasses) {
    for (int i = 0; i < numOfClasses; i++) {
        shared_ptr<Lecture> ptr(new Lecture(i, course_id, bottom_node));
        course_Lectures[i] = ptr;
        bottom_node->insert(course_id, i);
    }
}

shared_ptr<Lecture> CourseNode::get_class(int class_num) {
    return course_Lectures[class_num];
}

void CourseNode::remove() {
    try {
        for (int i = 0; i < numOfClasses; i++) {
            course_Lectures[i]->get_location()->remove(course_id, i);
        }
    } catch (...) {
        throw;
    }
}
}  // namespace LecturesStats
