#ifndef WET_HW1_COURSENODE_H
#define WET_HW1_COURSENODE_H

#include "Lecture.h"

#include <memory>

using namespace std;

class CourseNode {
private:
    shared_ptr<Lecture>* course_Lectures{};
    int course_id;
    int numOfClasses;
public:
    CourseNode(int numOfClasses, int course_id,ListNode* bottom_node);
    shared_ptr<Lecture> get_class(int class_num);

    int get_class_num() { return numOfClasses; }

    void remove();
};

#endif //WET_HW1_COURSENODE_H
