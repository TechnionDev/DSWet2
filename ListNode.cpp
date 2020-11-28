#include "ListNode.h"

void ListNode::insert(const shared_ptr<Lecture>& lecture) {
    try {
        shared_ptr<BinTree<int, shared_ptr<Lecture> > > course_tree_ptr = lecture_tree.get(lecture->get_course_id());
        if (course_tree_ptr == nullptr) {
            shared_ptr<BinTree<int, shared_ptr<Lecture>>> new_lecture_tree(new BinTree<int, shared_ptr<Lecture>>);
            lecture_tree.add(lecture->get_course_id(), new_lecture_tree);
            new_lecture_tree->add(lecture->get_lecture_id(), lecture);
        } else {
            course_tree_ptr->add(lecture->get_lecture_id(), lecture);
        }
    } catch (...) { throw; }
}

void ListNode::remove(const shared_ptr<Lecture>& lecture) {
    try {
        shared_ptr<BinTree<int, shared_ptr<Lecture> > > course_tree_ptr = lecture_tree.get(lecture->get_course_id());
        if (course_tree_ptr == nullptr) {
            assert("the location of the lecture points to the wrong node ");
        } else {
            course_tree_ptr->pop(lecture->get_lecture_id());
            if(course_tree_ptr->is_empty()){
                lecture_tree.pop(lecture->get_course_id());
            }
            if(lecture_tree.is_empty()){
                if(next_node!= nullptr){
                    next_node->set_prev_node(prev_node);
                }
                if(prev_node!= nullptr){
                    prev_node->set_next_node(next_node);
                }
                free(this);
            }
        }
    } catch (...) { throw; }
}

ListNode* ListNode::get_prev_node() {
    return prev_node;
}

ListNode* ListNode::get_next_node() {
    return next_node;
}

const BinTree<int, shared_ptr<BinTree<int, shared_ptr<Lecture>>>>::iterator& ListNode::get_begin_iterator() {
    return lecture_tree.begin();
}

const BinTree<int, shared_ptr<BinTree<int, shared_ptr<Lecture>>>>::iterator& ListNode::get_end_iterator() {
    return lecture_tree.end();
}

void ListNode::set_prev_node(ListNode* new_node) {
    prev_node = new_node;
}

void ListNode::set_next_node(ListNode* new_node) {
    new_node = new_node;
}

ListNode::ListNode(int num_of_views) :views_in_node(num_of_views){
}

int ListNode::get_num_of_views() {
    return views_in_node;
}
