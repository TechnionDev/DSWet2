#include <cassert>
#include <memory>
#include <string>

namespace LecturesStats {
using std::shared_ptr;
using std::string;

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

template <class K, class V>
class Node {
   private:
    K key;
    V value;
    shared_ptr<Node<K, V>> parent;
    shared_ptr<Node<K, V>> left;
    shared_ptr<Node<K, V>> right;

   public:
    Node(const K& key, const V& value, const Node<K, V>& left = nullptr,
         const Node<K, V>& right = nullptr)
        : key(key), value(value), left(left), right(right){};
    void setParent(const Node<K, V>& node) {
        this->parent = shared_ptr<Node<K, V>>(node);
    }
    void setLeft(const Node<K, V>& node) {
        this->left = shared_ptr<Node<K, V>>(node);
    }
    void setRight(const Node<K, V>& node) {
        this->right = shared_ptr<Node<K, V>>(node);
    }
    bool isLeaf() { return not getRight() and not getLeft(); }
    Node<K, V> getParent() {
        if (this->parent) {
            return this->parent;
        } else {
            throw NullException("Can't get parent of head");
        }
    }
    Node<K, V> getLeft() {
        if (this->left) {
            return this->left;
        } else {
            throw NullException("No left node");
        }
    }
    Node<K, V> getRight() {
        if (this->right) {
            return this->right;
        } else {
            throw NullException("No right node");
        }
    }
    V& getValue() { return value; }
};

template <class K, class V>
class BinTree {
   private:
    shared_ptr<Node<K, V>> head;
    shared_ptr<Node<K, V>> max;

   public:
    // Iterations
    class iterator {
       private:
        // TODO: Remove typedef enum state { RIGHT, SELF, END } State;
        // State state;
        shared_ptr<Node<K, V>> curr_node;
        shared_ptr<Node<K, V>> prev_node;
        iterator(shared_ptr<Node<K, V>> node)
            : curr_node(node), prev_node(nullptr) {}
        void rise() const {
            prev_node = nullptr;
            while (curr_node->getLeft() == prev_node) {
                if (curr_node->getParent() == nullptr) {
                    curr_node = nullptr;
                    return;
                    // TODO: throw OutOfBoundsException("Tree iterator out of
                    // bounds");
                }
                prev_node = curr_node;
                curr_node = curr_node->getParent();
            }
        }
        void dropRight() const {
            while (curr_node->getRight() != nullptr) {
                curr_node = curr_node->getRight();
            }
            prev_node = nullptr;
        }

        friend class BinTree;

       public:
        iterator& operator++() const {
            if (not curr_node) {
                throw OutOfBoundsException("Tree iterator out of bounds");
            }
            if (curr_node->isLeaf()) {
                rise();
            } else if (prev_node == curr_node->getRight()) {
                if (curr_node->getLeft()) {
                    curr_node = curr_node->getLeft();
                    dropRight();
                } else {
                    rise();
                }
            } else {
                throw NullException("Aaaaa. This shouldn't be possible.");
            }
            return *this;
        }
        K& operator*() const { return curr_node->getValue(); };
        iterator operator++(int);
        bool operator==(const iterator& it) const;
        bool operator!=(const iterator& it) const;
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;
    };
    iterator begin() const { return iterator(max); }
    iterator end() const { return iterator(nullptr); }
    BinTree(shared_ptr<Node<K, V>> head = nullptr) : head(head){};

    /**
     * @brief Get the value attached to the given key
     * @param key Key
     * @return const V& Value
     */
    const V& get(K key) { throw NullException("Empty Tree"); }
    /**
     * @brief Same as get but removes the element from the tree
     * @param key Key
     * @return const V& Value
     */
    const V& pop(K key) { throw NullException("Empty Tree"); }
    /**
     * @brief Add create a new tree node (sorted ofc)
     * @param key Key
     * @param value Value
     */
    void add(K key, V value) { throw NullException("Empty Tree"); }
};
}  // namespace LecturesStats