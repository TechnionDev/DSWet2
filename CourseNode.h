#ifndef WET_HW1_COURSENODE_H
#define WET_HW1_COURSENODE_H

#include <memory>

#include "Lecture.h"

namespace LecturesStats {
    using namespace std;

    class CourseNode {
    private:
        shared_ptr<Lecture>* course_Lectures_array;
        int course_id;
        int numOfClasses;

    public:
        CourseNode(int numOfClasses, int course_id, ListNode* head);
        shared_ptr<Lecture> get_class(int class_num);
        int get_class_num() { return numOfClasses; }
        void pop_lectures(ListNode* tail);
        ~CourseNode();
    };
}  // namespace LecturesStats

#endif  // WET_HW1_COURSENODE_H
