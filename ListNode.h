#ifndef WET_HW1_LISTNODE_H
#define WET_HW1_LISTNODE_H

#include "BinTree.h"

namespace LecturesStats {

    class ListNode {
    private:
        int views_in_node = 0;
        BinTree<int, BinTree<int, void*>> course_lecture_tree;
        ListNode* next = nullptr;
        ListNode* prev = nullptr;
    public:
        ListNode(int num_of_views);
        BinTree<int, BinTree<int, void*>>::iterator get_begin_iterator();
        BinTree<int, BinTree<int, void*>>::iterator get_end_iterator();
        shared_ptr<BinTree<int, void*>> get_lecture_tree_ptr(int course_id);
        int get_num_of_views();
        ListNode* get_next_node();
        ListNode* get_prev_node();
        void set_next_node(ListNode* new_node);
        void set_prev_node(ListNode* new_node);
        void insert(int course_id, int lecture_id);
        void insert(int course_id, int bottom_range, int top_range);
        void remove(int course_id, int lecture_id, ListNode* (& tail));
    };
}  // namespace LecturesStats

#endif  // WET_HW1_LISTNODE_H
