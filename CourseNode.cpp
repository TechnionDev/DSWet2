#include "CourseNode.h"

namespace LecturesStats {
    CourseNode::CourseNode(int numOfClasses, int course_id, ListNode* head)
            : course_id(course_id), numOfClasses(numOfClasses) {
        shared_ptr<Lecture>* course_Lectures_ptr(new shared_ptr<Lecture>[numOfClasses]);
        course_Lectures_array = course_Lectures_ptr;

        head->insert(course_id,0,numOfClasses);
        for (int i = 0; i < numOfClasses; i++) {
            shared_ptr<Lecture> ptr(new Lecture(i, course_id, head));
            course_Lectures_array[i] = ptr;
        }
    }

    shared_ptr<Lecture> CourseNode::get_class(int class_num) {
        return course_Lectures_array[class_num];
    }

    CourseNode::~CourseNode() {
        delete[] course_Lectures_array;
    }

    void CourseNode::pop_lectures(ListNode* (& tail)) {
        for (int i = 0; i < numOfClasses; i++) {
            course_Lectures_array[i]->get_location()->remove(course_id, i, tail);
        }
    }
}  // namespace LecturesStats
