#ifndef WET_HW1_BINTREE_H
#define WET_HW1_BINTREE_H

#include <algorithm>
#include <cassert>

#include "BinTreePrototypes.h"

#define TREE_PRINT_SPREAD 10

namespace LecturesStats {
// using std::cout;  // DEBUG
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::ostream;

    using std::max;
    using std::shared_ptr;
    using std::string;
    using std::to_string;

    template<class K, class V>
    void Node<K, V>::setParent(Node<K, V>* parent) {
        this->parent = parent;
    }

    template<class K, class V>
    short Node<K, V>::balance() {
        int lheight = -1, rheight = -1;
        if (right) rheight = right->height;
        if (left) lheight = left->height;
        return lheight - rheight;
    }

    template<class K, class V>
    Node<K, V>::Node(const K& key, shared_ptr<V> value, Node<K, V>* left, Node<K, V>* right)
            : key(key),
              value(value),
              height(0),
              size(0),
              parent(NULL),
              left(left),
              right(right) {
        int lheight = -1, rheight = -1;
        int lsize = 0, rsize = 0;
        if (right) {
            rheight = right->height;
            rsize = right->size;
        }
        if (left) {
            lheight = left->height;
            lsize = left->size;
        }
        height = max(lheight, rheight) + 1;
        size = lsize + rsize + 1;

    }

    template<class K, class V>
    void Node<K, V>::setLeft(Node<K, V>* node) {
        left = node;
        if (node) {
            node->setParent(this);
        }
        int lheight = -1, rheight = -1;
        int lsize = 0, rsize = 0;
        if (right) {
            rheight = right->height;
            rsize = right->size;
        }

        if (left) {
            lheight = left->height;
            lsize = left->size;
        }

        height = max(lheight, rheight) + 1;
        size = lsize + rsize + 1;
    }

    template<class K, class V>
    void Node<K, V>::setRight(Node<K, V>* node) {
        this->right = node;
        if (node) {
            node->setParent(this);
        }
        int lheight = -1, rheight = -1;
        int lsize = 0, rsize = 0;
        if (right) {
            rheight = right->height;
            rsize = right->size;
        }

        if (left) {
            lheight = left->height;
            lsize = left->size;
        }

        height = max(lheight, rheight) + 1;
        size = rsize + lsize + 1;
    }

    template<class K, class V>
    bool Node<K, V>::isLeaf() {
        return not getRight() and not getLeft();
    }

    template<class K, class V>
    Node<K, V>* Node<K, V>::getParent() {
        return this->parent;
    }

    template<class K, class V>
    Node<K, V>* Node<K, V>::getLeft() {
        return this->left;
    }

    template<class K, class V>
    Node<K, V>* Node<K, V>::getRight() {
        return this->right;
    }

    template<class K, class V>
    shared_ptr<V> Node<K, V>::getValue() {
        return value;
    }

    template<class K, class V>
    const K& Node<K, V>::getKey() {
        return key;
    }

    template<class K, class V>
    const int Node<K, V>::getSize() {
        return size;
    }

    template<class K, class V>
    void BinTree<K, V>::iterator::rise() {
        prev = NULL;
        while (curr->getRight() == prev) {
            if (curr->getParent() == NULL) {
                curr = NULL;
                return;
            }
            prev = curr;
            curr = curr->getParent();
        }
    }

    template<class K, class V>
    void BinTree<K, V>::iterator::dropLeft() {
        while (curr->getLeft() != NULL) {
            curr = curr->getLeft();
        }
        prev = NULL;
    }

    template<class K, class V>
    BinTree<K, V>::iterator::iterator(Node<K, V>* node) : curr(node), prev(NULL) {}

    template<class K, class V>
    typename BinTree<K, V>::iterator& BinTree<K, V>::iterator::operator++() {
        if (not curr) {
            throw OutOfBoundsException("Tree iterator out of bounds");
        }
        if (curr->isLeaf()) {
            rise();
        } else {
            assert(prev == curr->getLeft());
            if (curr->getRight()) {
                curr = curr->getRight();
                dropLeft();
            } else {
                rise();
            }
        }
        return *this;
    }

    template<class K, class V>
    typename BinTree<K, V>::iterator BinTree<K, V>::iterator::operator++(int) {
        iterator it = *this;
        ++*this;
        return it;
    }

    template<class K, class V>
    const shared_ptr<V> BinTree<K, V>::iterator::value() const {
        return curr->getValue();
    }

    template<class K, class V>
    const K& BinTree<K, V>::iterator::key() const {
        return curr->getKey();
    }

    template<class K, class V>
    bool BinTree<K, V>::iterator::operator==(const iterator& it) const {
        return this->curr == it.curr;
    }

    template<class K, class V>
    bool BinTree<K, V>::iterator::operator!=(const iterator& it) const {
        return not(*this == it);
    }

    template<class K, class V>
    typename BinTree<K, V>::iterator BinTree<K, V>::begin() const {
        return iterator(min_node);
    }

    template<class K, class V>
    typename BinTree<K, V>::iterator BinTree<K, V>::end() const {
        return iterator(NULL);
    }

    template<class K, class V>
    BinTree<K, V>::BinTree() : head(nullptr) {}

    template<class K, class V>
    BinTree<K, V>::BinTree(int size) : BinTree() {
        this->head = fromRange(0, size - 1);
        min_node = find(0);
        max_node = find(size - 1);
        assert(isTreeStructured());
    }

    template<class K, class V>
    bool BinTree<K, V>::isEmpty() {
        return not head;
    }

    template<class K, class V>
    shared_ptr<V> BinTree<K, V>::get(const K& key) {
        Node<K, V>* n = find(key);
        return n ? n->getValue() : nullptr;
    }

    template<class K, class V>
    shared_ptr<V> BinTree<K, V>::getN(int index) {
        Node<K, V>* n = findN(index);
        return n ? n->getValue() : nullptr;
    }

#ifndef NDEBUG

    template<class K, class V>
    void BinTree<K, V>::print() {
        cout << head;
    };
#endif

    template<class K, class V>
    Node<K, V>* BinTree<K, V>::fromRange(int low, int high) {
        if (low > high) return NULL;
        // If only one cell left
        if (low == high) return new Node<K, V>(low, nullptr);
        // The average index is the cell we'll put as root for subtree
        int mid = (high + low) / 2;
        Node<K, V>* curr = new Node<K, V>(mid, nullptr);
        try {  // We want to make sure we catch bad allocations and handle it
            // well
            curr->setRight(fromRange(mid + 1, high));
            curr->setLeft(fromRange(low, mid - 1));
        } catch (std::bad_alloc exc) {
            // We don't need to delete the left because if it was assigned, then
            // no later allocations could have failed
            delete curr->getRight();
            delete curr;
            throw exc;
        }
        return curr;
    }

    template<class K, class V>
    Node<K, V>* BinTree<K, V>::find(const K& key) {
        Node<K, V>* curr = head;
        while (curr != NULL) {
            if (curr->getKey() == key) {
                return curr;
            } else if (curr->getKey() < key) {
                curr = curr->getRight();
            } else {
                curr = curr->getLeft();
            }
        }
        return NULL;
    }

    template<class K, class V>
    Node<K, V>* BinTree<K, V>::findN(int index) {
        if (not head) {
            return NULL;
        }
        Node<K, V>* curr = head;
        int size_count = 1;
        //init size_count with the rank of head (rank := size of left + 1)
        if (curr->getLeft()) {
            size_count = curr->getLeft()->getSize();
        }
        while (curr != NULL) {
            if (size_count == index) {
                return curr;
            } else if (size_count < index) {
                curr = curr->getRight();
                if (curr) {
                    size_count++;
                    if (curr->getLeft()) {
                        size_count += curr->getLeft()->getSize();
                    }
                }
            } else {
                if (curr->getLeft()) {
                    size_count -= curr->getLeft()->getSize() + 1;
                    curr = curr->getLeft();
                    if (curr->getLeft()) {
                        size_count += curr->getLeft()->getSize() + 1;
                    }
                } else {
                    curr = curr->getLeft();
                }
            }
        }
        return NULL;
    }

    template<class K, class V>
    void BinTree<K, V>::addRebalanceFrom(Node<K, V>* curr) {
        // Re-Balance the tree
        int lheight = -1, rheight = -1;
        int lsize = 0, rsize = 0;
        if (curr == NULL) return;

        while (curr != head) {
            Node<K, V>* parent = curr->getParent();
            assert(parent != NULL);  // because curr!=head
            if (parent->height > curr->height + 1) {
                updateSizeUp(parent);
                return;
            }
            rotateAsNeeded(parent);

            assert(abs(parent->balance()) < 2);
            if (parent->right) {
                rheight = parent->right->height;
                rsize = parent->right->size;
            }
            if (parent->left) {
                lheight = parent->left->height;
                lsize = parent->left->size;
            }
            parent->height = max(lheight, rheight) + 1;
            parent->size = lsize + rsize + 1;
            curr = parent;
        }
    }

    template<class K, class V>
    void BinTree<K, V>::popRebalanceFrom(Node<K, V>* curr) {
        // Re-Balance the tree
        int lheight = -1, rheight = -1;
        int lsize = 0, rsize = 0;
        while (curr) {
            Node<K, V>* parent = curr->getParent();
            rotateAsNeeded(curr);
            assert(abs(curr->balance()) < 2);
            // Update height & size
            if (curr->right) {
                rheight = curr->right->height;
                rsize = curr->right->size;
            }
            if (curr->left) {
                lheight = curr->left->height;
                lsize = curr->left->size;
            }
            curr->height = max(lheight, rheight) + 1;
            curr->size = lsize + rsize + 1;
            curr = parent;
        }
    }

    template<class K, class V>
    bool BinTree<K, V>::rotateAsNeeded(Node<K, V>* (& curr)) {
        if (curr->balance() == 2) {
            if (curr->getLeft()->balance() >= 0) {
                rotateLL(curr);
            } else {
                assert(curr->getLeft()->balance() == -1);
                rotateLR(curr);
            }
        } else if (curr->balance() == -2) {
            if (curr->getRight()->balance() <= 0) {
                rotateRR(curr);
            } else {
                assert(curr->getRight()->balance() == 1);
                rotateRL(curr);
            }
        } else {
            return false;
        }
        return true;
    }

    template<class K, class V>
    shared_ptr<V> BinTree<K, V>::pop(const K& key) {
        Node<K, V>* curr = head;

        if (not head) {
            return nullptr;
        }
        // Find pop location
        while (curr) {
            if (curr->getKey() < key) {
                curr = curr->getRight();
            } else if (curr->getKey() > key) {
                curr = curr->getLeft();
            } else /* (curr->getKey() == key) */ {
                break;
            }
        }
        if (not curr) {
            return nullptr;
        }

        Node<K, V>* node = curr;
        Node<K, V>* parent = NULL;
        Node<K, V>* next_in_order;
        Node<K, V>* ret_node = node;

        // Update max if needed
        if (ret_node == min_node) {
            if (min_node->isLeaf()) {
                min_node = min_node->getParent();
            } else {
                // Left == bigger (wouldn't be min)
                assert(min_node->getLeft() == NULL);
                // Right must be leaf, otherwise, height would be unbalanced
                assert(min_node->getRight()->isLeaf());  // Because it's an AVL
                // If we're removing min and max isn't a leaf, then it has only the
                // Right child which must be a leaf
                min_node = min_node->getRight();
            }
        }
        if (ret_node == max_node) {
            if (max_node->isLeaf()) {
                max_node = max_node->getParent();
            } else {
                // right == bigger (wouldn't be max)
                assert(max_node->getRight() == NULL);
                // Left must be leaf, otherwise, height would be unbalanced
                assert(max_node->getLeft()->isLeaf());  // Because it's an AVL
                // If we're removing max and max isn't a leaf, then it has only the
                // left child which must be a leaf
                max_node = max_node->getLeft();
            }
        }
        // Remove the node
        if (node->isLeaf()) {
            if (node == head) {
                head = NULL;
                curr = NULL;
            } else {
                parent = node->getParent();
                if (parent->getRight() == node) {
                    parent->setRight(NULL);
                } else {
                    assert(parent->getLeft() == node);
                    parent->setLeft(NULL);
                }
            }
            // delete node; Can't delete because we return it
            curr = parent;
        } else if (node->getLeft() and (curr = node->getRight())) {
            // Get next in order
            parent = node;
            next_in_order = curr;
            while (next_in_order->getLeft()) {
                parent = next_in_order;
                next_in_order = next_in_order->getLeft();
            }
            // Detach next_in_order from tree (to be added later instead of node)
            if (parent->getLeft() == next_in_order) {
                parent->setLeft(next_in_order->getRight());
            } else {
                assert(parent->getRight() == next_in_order);
                parent->setRight(next_in_order->getRight());
            }

            // Re-Add next_in_order instead of node
            //   Take node's place
            next_in_order->setRight(node->getRight());
            next_in_order->setLeft(node->getLeft());
            next_in_order->setParent(node->getParent());
            //   Make sure no parent and node==head are mutually exclusive
            assert((not node->getParent()) == (node == head));
            assert(parent != NULL);
            if (node != head) {
                if (node->getParent()->getLeft() == node) {
                    node->getParent()->setLeft(next_in_order);
                } else {
                    assert(node->getParent()->getRight() == node);
                    node->getParent()->setRight(next_in_order);
                }
            } else {
                head = next_in_order;
            }
            curr = parent == node ? next_in_order : parent;
        } else /* One child */ {
            Node<K, V>* child =
                    node->getLeft() ? node->getLeft() : node->getRight();
            parent = node->getParent();
            // Make sure no parent and node==head are mutually exclusive
            assert((not parent) == (node == head));
            if (not parent) {
                head = child;
                child->parent = NULL;
            } else {
                if (parent->getLeft() == node) {
                    parent->setLeft(child);
                } else {
                    assert(parent->getRight() == node);
                    parent->setRight(child);
                }
            }
            curr = child;
        }

        popRebalanceFrom(curr);
        ret_node->parent = NULL;
        assert(isTreeStructured());
        auto value = ret_node->value;
        delete ret_node;
        return value;
    }

    template<class K, class V>
    void BinTree<K, V>::add(const K& key, shared_ptr<V> value) {
        Node<K, V>* curr = head;
        Node<K, V>* prev = NULL;
        Node<K, V>* new_node = new Node<K, V>(key, value);

        if (not head) {
            max_node = min_node = head = new_node;
            return;
        }
        // Find insert location
        while (curr) {
            prev = curr;
            if (curr->getKey() < key) {
                curr = curr->getRight();
            } else {
                if (curr->getKey() == key) {
                    delete new_node;
                    throw AlreadyExistException("Key " + to_string(key) +
                                                " already exist in tree");
                }
                curr = curr->getLeft();
            }
        }
        curr = prev;

        // Add the new node
        if (curr->getKey() < key) {
            curr->setRight(new_node);
        } else {
            curr->setLeft(new_node);
        }

        // Update max&min node if needed
        if (not min_node or min_node->getKey() > key) {
            min_node = new_node;
        }
        if (not max_node or max_node->getKey() < key) {
            min_node = new_node;
        }
        // Balance the tree
        addRebalanceFrom(curr);
        assert(isTreeStructured());
    }

    template<class K, class V>
    void BinTree<K, V>::rotateLL(Node<K, V>* (& root)) {
        // Names coresponding to lectures node names
        Node<K, V>* parent = root->parent;
        Node<K, V>* nodeB = root;
        Node<K, V>* nodeA = root->getLeft();
        nodeB->setLeft(nodeA->getRight());
        nodeA->setRight(nodeB);
        if (root == head) {
            root = head = nodeA;
            nodeA->setParent(NULL);
        } else {
            if (parent->right == root) {
                parent->setRight(nodeA);
            } else {
                parent->setLeft(nodeA);
            }
            root = nodeA;
        }
        // this->print();  // DEBUG
    }

    template<class K, class V>
    void BinTree<K, V>::rotateLR(Node<K, V>* (& root)) {
        // Names coresponding to lectures node names
        Node<K, V>* parent = root->parent;
        Node<K, V>* nodeC = root;
        Node<K, V>* nodeA = root->getLeft();
        Node<K, V>* nodeB = nodeA->getRight();
        nodeC->setLeft(nodeB->getRight());
        nodeB->setRight(nodeC);
        nodeA->setRight(nodeB->getLeft());
        nodeB->setLeft(nodeA);
        if (root == head) {
            root = head = nodeB;
            nodeB->setParent(NULL);
        } else {
            if (parent->right == root) {
                parent->setRight(nodeB);
            } else {
                parent->setLeft(nodeB);
            }
            root = nodeB;
        }
    }

    template<class K, class V>
    void BinTree<K, V>::rotateRR(Node<K, V>* (& root)) {
        // Names coresponding to lectures node names
        Node<K, V>* parent = root->parent;
        Node<K, V>* nodeB = root;
        Node<K, V>* nodeA = root->getRight();
        nodeB->setRight(nodeA->getLeft());
        nodeA->setLeft(nodeB);

        nodeA->parent = parent;
        if (root == head) {
            root = head = nodeA;
        } else {
            if (parent->right == root) {
                parent->setRight(nodeA);
            } else {
                parent->setLeft(nodeA);
            }
            root = nodeA;
        }
    }

    template<class K, class V>
    void BinTree<K, V>::rotateRL(Node<K, V>* (& root)) {
        // Names coresponding to lectures node names
        Node<K, V>* parent = root->parent;
        Node<K, V>* nodeC = root;
        Node<K, V>* nodeA = root->getRight();
        Node<K, V>* nodeB = nodeA->getLeft();
        nodeC->setRight(nodeB->getLeft());
        nodeB->setLeft(nodeC);
        nodeA->setLeft(nodeB->getRight());
        nodeB->setRight(nodeA);
        if (root == head) {
            root = head = nodeB;
            nodeB->setParent(NULL);
        } else {
            if (parent->right == root) {
                parent->setRight(nodeB);
            } else {
                parent->setLeft(nodeB);
            }
            root = nodeB;
        }
    }

    template<class K, class V>
    BinTree<K, V>::~BinTree() {
        if (head) {
            deallocTree(head);
        }
    }

    template<class K, class V>
    void BinTree<K, V>::deallocTree(Node<K, V>* curr) {
        if (not curr) {
            return;
        }
        deallocTree(curr->left);
        deallocTree(curr->right);
        delete curr;
    }

#ifndef NDEBUG

// Function to print binary tree in 2D
// It does reverse inorder traversal
    template<class K, class V>
    void Node<K, V>::print2DUtil(ostream& os, Node<K, V>* root, int space) {
        // Base case
        if (root == NULL) return;

        // Increase distance between levels
        space += TREE_PRINT_SPREAD;

        // Process right child first
        print2DUtil(os, root->right, space);

        // Print current node after space
        // count
        os << endl;
        for (int i = TREE_PRINT_SPREAD; i < space; i++) os << " ";
        os << root->key << ";" << to_string(root->height) << endl;

        // Process left child
        print2DUtil(os, root->left, space);
    }

    template<class K, class V>
    ostream& operator<<(ostream& os, Node<K, V>* node) {
        Node<K, V>::print2DUtil(os, node, 0);
        os << "\n" << endl;
        return os;
    }

    template<class K, class V>
    bool BinTree<K, V>::isTreeStructured() {
        // this->print(); // DEBUG
        return isTreeStructured(NULL, head);
    }

    template<class K, class V>
    bool BinTree<K, V>::isTreeStructured(Node<K, V>* parent, Node<K, V>* node) {
        bool lstatus = true, rstatus = true;
        Node<K, V>* tmp;
        if (node == NULL) return not parent or parent->height == 0;

        if (parent != node->getParent()) {
            cerr << "Parent check failed on node: " << to_string(node->getKey())
                 << endl
                 << "Expected parent: " << to_string(parent->getKey()) << endl
                 << "Got parent: " << to_string(node->getParent()->getKey())
                 << endl;
            return false;
        }

        if ((tmp = node->getLeft())) {
            if (tmp->getKey() >= node->getKey()) {
                cerr << "Sort check failed on node: " << to_string(node->getKey())
                     << "'s left." << endl;
                return false;
            }
            // TODO: Fix height verifications
            // else if (tmp->height + 1 >= node->height)
            //     return false;
            lstatus = isTreeStructured(node, tmp);
        }

        if ((tmp = node->getRight())) {
            if (tmp->getKey() <= node->getKey()) {
                cerr << "Check failed on node: " << to_string(node->getKey())
                     << "'s right." << endl;
                return false;
            }
            // TODO: Fix height verifications
            // else if (tmp->height + 1 >= node->height)
            //     return false;
            rstatus = isTreeStructured(node, tmp);
        }

        return lstatus && rstatus;
    }

    template<class K, class V>
    void BinTree<K, V>::updateSizeUp(Node<K, V>* curr) {
        // Update size form curr node upwards
        int lsize = 0, rsize = 0;
        if (curr == NULL) return;

        while (curr != head) {
            Node<K, V>* parent = curr->getParent();
            assert(parent != NULL);  // because curr!=head
            if (parent->right) {
                rsize = parent->right->size;
            }
            if (parent->left) {
                lsize = parent->left->size;
            }
            parent->size = lsize + rsize + 1;
            curr = parent;
        }
    }

    template<class K, class V>
    int BinTree<K, V>::sizeOfTree() {
        return head->size;
    }

    template<class K, class V>
    shared_ptr<V> BinTree<K, V>::getMax() {
        return max_node->getValue();
    }

    template<class K, class V>
    const K&  BinTree<K, V>::getMaxKey() {
        return max_node->getKey();
    }


#endif

}  // namespace LecturesStats
#endif