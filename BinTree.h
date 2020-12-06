#ifndef WET_HW1_BINTREE_H
#define WET_HW1_BINTREE_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

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

class Exception : public std::exception {
   protected:
    string error = "";

   public:
    Exception() : error("No message specified"){};
    Exception(string msg) : error(msg){};
    const char* what() const noexcept { return error.c_str(); }
};

class OutOfBoundsException : public Exception {
   public:
    OutOfBoundsException() = default;
    OutOfBoundsException(string msg) : Exception(msg){};
};

class AlreadyExistException : public Exception {
   public:
    AlreadyExistException() = default;
    AlreadyExistException(string msg) : Exception(msg){};
};

template <class K, class V>
class BinTree;

template <class K, class V>
class Node {
   private:
    K key;
    shared_ptr<V> value;
    int height;
    Node<K, V>* parent;
    Node<K, V>* left;
    Node<K, V>* right;

    void setParent(Node<K, V>* parent) { this->parent = parent; }
    short balance() {
        int lheight = -1, rheight = -1;
        if (right) rheight = right->height;
        if (left) lheight = left->height;
        return lheight - rheight;
    }
    friend class BinTree<K, V>;

   public:
    Node(const K& key, shared_ptr<V> value, Node<K, V>* left = NULL,
         Node<K, V>* right = NULL)
        : key(key),
          value(value),
          height(0),
          parent(NULL),
          left(left),
          right(right){};
    void setLeft(Node<K, V>* node) {
        left = node;
        if (node) {
            node->setParent(this);
        }
        int lheight = -1, rheight = -1;
        if (right) rheight = right->height;

        if (left) lheight = left->height;

        height = max(lheight, rheight) + 1;
    }
    void setRight(Node<K, V>* node) {
        this->right = node;
        if (node) {
            node->setParent(this);
        }
        int lheight = -1, rheight = -1;
        if (right) rheight = right->height;

        if (left) lheight = left->height;

        height = max(lheight, rheight) + 1;
    }
    bool isLeaf() { return not getRight() and not getLeft(); }
    Node<K, V>* getParent() { return this->parent; }
    Node<K, V>* getLeft() { return this->left; }
    Node<K, V>* getRight() { return this->right; }
    shared_ptr<V> getValue() { return value; }
    const K& getKey() { return key; }
#ifndef NDEBUG
    static void print2DUtil(ostream& os, Node<K, V>* root, int space);
#endif
};

template <class K, class V>
class BinTree {
   private:
    Node<K, V>* head;
    Node<K, V>* max_node = NULL;

    void rotateLL(Node<K, V>*(&node));
    void rotateLR(Node<K, V>*(&node));
    void rotateRR(Node<K, V>*(&node));
    void rotateRL(Node<K, V>*(&node));
    Node<K, V>* find(const K& key);
#ifndef NDEBUG
    // Validates the tree's structure. Makes sure (recursively) that everything
    // points where it should. For debug asserts
    bool isTreeStructured();
    static bool isTreeStructured(Node<K, V>* parent, Node<K, V>* node);
#endif

   public:
    // Iterations
    class iterator {
       private:
        Node<K, V>* curr;
        Node<K, V>* prev;
        iterator(Node<K, V>* node) : curr(node), prev(NULL) {}
        void rise() {
            prev = NULL;
            while (curr->getLeft() == prev) {
                if (curr->getParent() == NULL) {
                    curr = NULL;
                    return;
                    // TODO: throw OutOfBoundsException("Tree iterator out of
                    // bounds");
                }
                prev = curr;
                curr = curr->getParent();
            }
        }
        void dropRight() {
            while (curr->getRight() != NULL) {
                curr = curr->getRight();
            }
            prev = NULL;
        }

        friend class BinTree<K, V>;

       public:
        iterator& operator++() {
            if (not curr) {
                throw OutOfBoundsException("Tree iterator out of bounds");
            }
            if (curr->isLeaf()) {
                rise();
            } else {
                assert(prev == curr->getRight());
                if (curr->getLeft()) {
                    curr = curr->getLeft();
                    dropRight();
                } else {
                    rise();
                }
            }
            return *this;
        }
        iterator operator++(int) {
            iterator it = *this;
            ++*this;
            return it;
        }
        const shared_ptr<V> value() const { return curr->getValue(); }
        const K& key() const { return curr->getKey(); }
        bool operator==(const iterator& it) const {
            return this->curr == it.curr;
        }
        bool operator!=(const iterator& it) const { return not(*this == it); }
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;
    };
    iterator begin() const { return iterator(max_node); }
    iterator end() const { return iterator(NULL); }
    BinTree() : head(nullptr){};
    bool isEmpty() { return not head; }
    ~BinTree();
    void deallocTree(Node<K, V>* curr);

    /**
     * @brief Get the value attached to the given key
     * @param key Key
     * @return shared_ptr<V> Value
     */
    shared_ptr<V> get(const K& key) {
        Node<K, V>* n = find(key);
        return n ? n->getValue() : nullptr;
    }
    /**
     * @brief Same as get but removes the element from the tree
     * @param key Key
     * @return shared_ptr<V> Value
     */
    // TODO: Consider returning a node pointer because we remove the node itself
    // from the tree so reference to the destroyed node will not mean much
    shared_ptr<V> pop(const K& key);
    /**
     * @brief Add create a new tree node (sorted ofc)
     * @param key Key
     * @param value Value
     */
    void add(const K& key, shared_ptr<V> value);
    void addRebalanceFrom(Node<K, V>* curr);
    void popRebalanceFrom(Node<K, V>* curr);
    // Make a rotation. Return whether a rotation took place
    bool rotateAsNeeded(Node<K, V>*(&curr));
#ifndef NDEBUG
    void print() { cout << head; };
#endif
};

template <class K, class V>
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

template <class K, class V>
void BinTree<K, V>::addRebalanceFrom(Node<K, V>* curr) {
    // Re-Balance the tree
    int lheight = -1, rheight = -1;
    if (curr == NULL) return;

    while (curr != head) {
        Node<K, V>* parent = curr->getParent();
        assert(parent != NULL);  // because curr!=head
        if (parent->height > curr->height + 1) {
            return;
        }
        parent->height = curr->height + 1;
        rotateAsNeeded(parent);

        assert(abs(parent->balance()) < 2);
        if (parent->right) rheight = parent->right->height;
        if (parent->left) lheight = parent->left->height;
        parent->height = max(lheight, rheight) + 1;
        curr = parent;
    }
}

template <class K, class V>
void BinTree<K, V>::popRebalanceFrom(Node<K, V>* curr) {
    // Re-Balance the tree
    int lheight = -1, rheight = -1;

    while (curr) {
        Node<K, V>* parent = curr->getParent();
        rotateAsNeeded(curr);
        assert(abs(curr->balance()) < 2);
        // Update height
        if (curr->right) rheight = curr->right->height;
        if (curr->left) lheight = curr->left->height;
        curr->height = max(lheight, rheight) + 1;
        curr = parent;
    }
}

template <class K, class V>
bool BinTree<K, V>::rotateAsNeeded(Node<K, V>*(&curr)) {
    if (curr->balance() == 2) {  // DEBUG
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

template <class K, class V>
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
    if (ret_node == max_node) {
        if (max_node->isLeaf()) {
            max_node = max_node->getParent();
        } else {
            // right == bigger (wouldn't be max)
            assert(max_node->getRight() == NULL);
            // Left must be leaf, otherwise, height would be unbalanced
            assert(max_node->getLeft()->isLeaf());
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

template <class K, class V>
void BinTree<K, V>::add(const K& key, shared_ptr<V> value) {
    Node<K, V>* curr = head;
    Node<K, V>* prev = NULL;
    Node<K, V>* new_node = new Node<K, V>(key, value);

    if (not head) {
        max_node = head = new_node;
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

    // Update max node if needed
    if (not max_node or max_node->getKey() < key) {
        max_node = new_node;
    }

    // Balance the tree
    addRebalanceFrom(curr);
    assert(isTreeStructured());
}

template <class K, class V>
void BinTree<K, V>::rotateLL(Node<K, V>*(&root)) {
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

template <class K, class V>
void BinTree<K, V>::rotateLR(Node<K, V>*(&root)) {
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

template <class K, class V>
void BinTree<K, V>::rotateRR(Node<K, V>*(&root)) {
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

template <class K, class V>
void BinTree<K, V>::rotateRL(Node<K, V>*(&root)) {
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

template <class K, class V>
BinTree<K, V>::~BinTree() {
    if (head) {
        deallocTree(head);
    }
}

template <class K, class V>
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
template <class K, class V>
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

template <class K, class V>
ostream& operator<<(ostream& os, Node<K, V>* node) {
    Node<K, V>::print2DUtil(os, node, 0);
    os << "\n" << endl;
    return os;
}

template <class K, class V>
bool BinTree<K, V>::isTreeStructured() {
    // this->print(); // DEBUG
    return isTreeStructured(NULL, head);
}

template <class K, class V>
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
#endif

}  // namespace LecturesStats
#endif