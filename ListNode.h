#ifndef WET_HW1_LISTNODE_H
#define WET_HW1_LISTNODE_H

#include "Lecture.h"
#include "BinTree_GUR.h"
#include <memory>

using namespace std;

class ListNode {
private:
    int views_in_node = 0;
    BinTree<int, shared_ptr<BinTree<int, shared_ptr<Lecture> > > > lecture_tree;
    ListNode* next_node = nullptr;
    ListNode* prev_node = nullptr;
public:
    ListNode(int num_of_views);
    const BinTree<int, shared_ptr<BinTree<int, shared_ptr<Lecture>>>>::iterator& get_begin_iterator();
    const BinTree<int, shared_ptr<BinTree<int, shared_ptr<Lecture>>>>::iterator& get_end_iterator();
    int get_num_of_views();
    ListNode* get_next_node();
    ListNode* get_prev_node();
    void set_next_node(ListNode* new_node);
    void set_prev_node(ListNode* new_node);
    void insert(const shared_ptr<Lecture>& lecture);
    void remove(const shared_ptr<Lecture>& lecture);
};


#endif //WET_HW1_LISTNODE_H
