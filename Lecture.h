#ifndef WET_HW1_LECTURE_H
#define WET_HW1_LECTURE_H

#include "ListNode.h"

namespace LecturesStats {
    class Lecture {
        int views = 0;
        int lecture_id;
        int course_id;
        ListNode* location;
        void change_location(int num_views, ListNode* (& tail));

    public:
        Lecture(int lecture_id, int course_id, ListNode* head);
        int get_views();
        void add_views(
                int num_views,
                ListNode* (& tail));
        ListNode* get_location();
    };

}  // namespace LecturesStats
#endif  // WET_HW1_LECTURE_H
