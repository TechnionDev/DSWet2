#ifndef WET_HW1_LECTURE_H
#define WET_HW1_LECTURE_H

#include "ListNode.h"

class Lecture {
    int views = 0;
    int lecture_id;
    int course_id;
    ListNode* location;
    void change_location(int num_views,ListNode* top_node);
public:
    Lecture(int lecture_id, int course_id,ListNode* bottom_node);
    int get_course_id();
    int get_lecture_id();
    int get_views();
    void add_views(int num_views,ListNode* top_node);//todo::maybe need to return status and not void
    ListNode* get_location();
};


#endif //WET_HW1_LECTURE_H
