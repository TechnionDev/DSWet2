#include <algorithm>
#include <cassert>
#include <memory>
#include <string>

namespace LecturesStats {
using std::max;
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

class NullException : public Exception {
   public:
    NullException() = default;
    NullException(string msg) : Exception(msg){};
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

class NotFoundException : public Exception {
   public:
    NotFoundException() = default;
    NotFoundException(string msg) : Exception(msg){};
};

template <class K, class V>
class Node {
   private:
    K key;
    V value;
    int height;
    short balance;
    Node<K, V>* parent;
    Node<K, V>* left;
    Node<K, V>* right;

    void setParent(Node<K, V>* parent) { this->parent = parent; }
    void updateStatsUp() {
        Node<K, V>* node = this;

        int lheight = -1, rheight = -1;
        while (node != NULL) {
            if (node->right) {
                rheight = node->right->height();
            }
            if (node->left) {
                lheight = node->left->height();
            }
            node->balance = lheight - rheight;
            node->height = max(lheight, rheight) + 1;
            node = node->getParent();
        }
    }

   public:
    Node(const K& key, const V& value, const Node<K, V>& left = NULL,
         const Node<K, V>& right = NULL)
        : key(key), value(value), left(left), right(right), height(0){};
    void setLeft(Node<K, V>* node) {
        this->left = node;
        if (node) {
            node->setParent(this);
        }
    }
    void setRight(Node<K, V>* node) {
        this->right = node;
        if (node) {
            node->setParent(this);
        }
    }
    bool isLeaf() { return not getRight() and not getLeft(); }
    Node<K, V> getParent() {
        if (this->parent) {
            return this->parent;
        } else {
            throw NullException("Can't get parent of head");
        }
    }
    Node<K, V>* getLeft() {
        if (this->left) {
            return this->left;
        } else {
            throw NullException("No left node");
        }
    }
    Node<K, V>* getRight() {
        if (this->right) {
            return this->right;
        } else {
            throw NullException("No right node");
        }
    }
    V& getValue() { return value; }
    const K& getKey() { return key; }
};

template <class K, class V>
class BinTree {
   private:
    Node<K, V>* head;
    Node<K, V>* max;

    void rotateLL(Node<K, V>* node);
    void rotateLR(Node<K, V>* node);
    void rotateRR(Node<K, V>* node);
    void rotateRL(Node<K, V>* node);
    Node<K, V>* find(const K& key);

   public:
    // Iterations
    class iterator {
       private:
        Node<K, V>* curr;
        Node<K, V>* prev;
        iterator(Node<K, V>* node) : curr(node), prev(NULL) {}
        void rise() const {
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
        void dropRight() const {
            while (curr->getRight() != NULL) {
                curr = curr->getRight();
            }
            prev = NULL;
        }

        friend class BinTree;

       public:
        iterator& operator++() const {
            if (not curr) {
                throw OutOfBoundsException("Tree iterator out of bounds");
            }
            if (curr->isLeaf()) {
                rise();
            } else if (prev == curr->getRight()) {
                if (curr->getLeft()) {
                    curr = curr->getLeft();
                    dropRight();
                } else {
                    rise();
                }
            } else {
                throw NullException("Aaaaa. This shouldn't be possible.");
            }
            return *this;
        }
        K& operator*() const { return curr->getValue(); };
        iterator operator++(int);
        bool operator==(const iterator& it) const {
            return this->curr == it.curr;
        }
        bool operator!=(const iterator& it) const { return not(*this == it); }
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;
    };
    iterator begin() const { return iterator(max); }
    iterator end() const { return iterator(NULL); }
    BinTree(Node<K, V>* head = NULL) : head(head){};

    /**
     * @brief Get the value attached to the given key
     * @param key Key
     * @return const V& Value
     */
    V& get(const K& key) { return find(key)->getValue(); }
    /**
     * @brief Same as get but removes the element from the tree
     * @param key Key
     * @return const V& Value
     */
    // TODO: Consider returning a node pointer because we remove the node itself
    // from the tree so reference to the destroyed node will not mean much
    V& pop(const K& key);
    /**
     * @brief Add create a new tree node (sorted ofc)
     * @param key Key
     * @param value Value
     */
    void add(const K& key, const V& value);
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
    throw NotFoundException("Key " + to_string(key) + " not found");
}

template <class K, class V>
V& BinTree<K, V>::pop(const K& key) {
    Node<K, V>* node = find(key);
    if (node) {
        // Remove from parent
        Node<K, V>* parent = node->getParent();
        if (parent) {
            if (parent->getRight() == node) {
                parent->setRight(NULL);
            } else {
                assert(parent->getLeft() == node);
                parent->setLeft(NULL);
            }
        }
        // TODO: Complete according to AVL remove algo
    }
}

template <class K, class V>
void BinTree<K, V>::add(const K& key, const V& value) {
    Node<K, V>* curr = head;
    Node<K, V>* prev = NULL;
    Node<K, V>* new_node = new Node<K, V>(key, value);

    if (not head) {
        head = new_node;
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

    // Balance the tree
    curr->updateStatsUp();
    while (curr) {
        // TODO: Finish the balancing task
        if (curr->balance == 2) {

        } else if (curr->balance == -2) {
        }
        // TODO: Check if we might need multiple rotations
        curr = curr->getParent();
    }
}

template <class K, class V>
void BinTree<K, V>::rotateLL(Node<K, V>* root) {
    // Names coresponding to lectures node names
    Node<K, V>* nodeB = root;
    Node<K, V>* nodeA = root->getLeft();
    nodeB->setLeft(nodeA->getRight());
    nodeA->setRight(nodeB);
    root = nodeA;
}

template <class K, class V>
void BinTree<K, V>::rotateLR(Node<K, V>* node) {
    // Names coresponding to lectures node names
    Node<K, V>* nodeC = node;
    Node<K, V>* nodeA = node->getLeft();
    Node<K, V>* nodeB = nodeA->getRight();
    nodeC->setLeft(nodeB->getRight());
    nodeB->setRight(nodeC);
    nodeA->setRight(nodeB->getLeft());
    nodeB->setLeft(nodeA);
    node = nodeB;
}

template <class K, class V>
void BinTree<K, V>::rotateRR(Node<K, V>* root) {
    // Names coresponding to lectures node names
    Node<K, V>* nodeB = root;
    Node<K, V>* nodeA = root->getRight();
    nodeB->setRight(nodeA->getLeft());
    nodeA->setLeft(nodeB);
    root = nodeA;
}

template <class K, class V>
void BinTree<K, V>::rotateRL(Node<K, V>* node) {
    // Names coresponding to lectures node names
    Node<K, V>* nodeC = node;
    Node<K, V>* nodeA = node->getRight();
    Node<K, V>* nodeB = nodeA->getLeft();
    nodeC->setRight(nodeB->getLeft());
    nodeB->setLeft(nodeC);
    nodeA->setLeft(nodeB->getRight());
    nodeB->setRight(nodeA);
    node = nodeB;
}
}  // namespace LecturesStats