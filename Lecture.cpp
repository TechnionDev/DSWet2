#include "Lecture.h"

namespace LecturesStats {
    Lecture::Lecture(int lecture_id, int course_id, ListNode* head)
            : lecture_id(lecture_id), course_id(course_id), location(head) {}

    void Lecture::add_views(int num_views, ListNode* (& tail)) {
        try {
            views += num_views;
            change_location(num_views, tail);
        } catch (...) {
            throw;
        }
    }

    void Lecture::change_location(int num_views, ListNode* (& tail)) {
        try {
            ListNode* temp_node = location;
            for (int i = num_views; i > 0; i--) {
                if (temp_node->get_num_of_views() == this->get_views()) {
                    temp_node->insert(course_id, lecture_id);
                    location->remove(course_id, lecture_id, tail);
                    location = temp_node;
                    break;
                }
                else if (temp_node->get_next_node() == nullptr) {
                    ListNode* new_node(new ListNode(this->get_views()));
                    tail = new_node;
                    temp_node->set_next_node(new_node);
                    new_node->set_prev_node(temp_node);
                    new_node->insert(course_id, lecture_id);
                    location->remove(course_id, lecture_id, tail);
                    location = new_node;
                    break;
                }
                else if (temp_node->get_next_node()->get_num_of_views() > this->get_views()) {
                    ListNode* new_node(new ListNode(this->get_views()));
                    new_node->set_next_node(temp_node->get_next_node());
                    new_node->set_prev_node(temp_node);
                    temp_node->get_next_node()->set_prev_node(new_node);
                    temp_node->set_next_node(new_node);
                    new_node->insert(course_id,lecture_id);
                    location->remove(course_id,lecture_id,tail);
                    location = new_node;
                    break;
                }
                temp_node = temp_node->get_next_node();
            }
        } catch (...) {
            throw;
        }
    }

    int Lecture::get_views() { return views; }

    ListNode* Lecture::get_location() { return location; }

}  // namespace LecturesStats