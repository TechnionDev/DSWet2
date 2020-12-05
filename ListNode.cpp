#include "ListNode.h"

namespace LecturesStats {

void ListNode::insert(int course_id, int lecture_id) {
    try {
        shared_ptr<BinTree<int, void*>> lecture_tree_ptr =
            course_lecture_tree.get(course_id);
        if (lecture_tree_ptr == nullptr) {
            shared_ptr<BinTree<int, void*>> new_lecture_tree(
                new BinTree<int, void*>);
            course_lecture_tree.add(course_id, new_lecture_tree);
            new_lecture_tree->add(lecture_id, nullptr);
        } else {
            lecture_tree_ptr->add(lecture_id, nullptr);
        }
    } catch (...) {
        throw;
    }
}

void ListNode::remove(int course_id, int lecture_id) {
    try {
        shared_ptr<BinTree<int, void*>> lecture_tree_ptr =
            course_lecture_tree.get(course_id);
        if (lecture_tree_ptr == nullptr) {
            assert("the location of the lecture points to the wrong node ");
        } else {
            lecture_tree_ptr->pop(lecture_id);
            if (lecture_tree_ptr->isEmpty()) {
                course_lecture_tree.pop(course_id);
            }
            if (course_lecture_tree.isEmpty()) {
                if (next != nullptr) {
                    next->set_prev_node(prev);
                }
                if (prev != nullptr) {
                    prev->set_next_node(next);
                }
                delete this;
            }
        }
    } catch (...) {
        throw;
    }
}

ListNode* ListNode::get_prev_node() { return prev; }

ListNode* ListNode::get_next_node() { return next; }

void ListNode::set_prev_node(ListNode* new_node) { prev = new_node; }

void ListNode::set_next_node(ListNode* new_node) { next = new_node; }

ListNode::ListNode(int num_of_views) : views_in_node(num_of_views) {}

int ListNode::get_num_of_views() { return views_in_node; }

BinTree<int, BinTree<int, void*>>::iterator ListNode::get_end_iterator() {
    return course_lecture_tree.end();
}

BinTree<int, BinTree<int, void*>>::iterator ListNode::get_begin_iterator() {
    return course_lecture_tree.begin();
}
}  // namespace LecturesStats
