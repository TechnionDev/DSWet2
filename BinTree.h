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
    };
    void setLeft(const Node<K, V>& node) {
        this->left = shared_ptr<Node<K, V>>(node);
    };
    void setRight(const Node<K, V>& node) {
        this->right = shared_ptr<Node<K, V>>(node);
    };
    Node<K, V> getParent() {
        if (this->parent) {
            return this->parent;
        } else {
            throw NullException("Can't get parent of head");
        }
    };
    Node<K, V> getLeft() {
        if (this->left) {
            return this->left;
        } else {
            throw NullException("No left node");
        }
    };
    Node<K, V> getRight() {
        if (this->right) {
            return this->right;
        } else {
            throw NullException("No right node");
        }
    };
};

template <class K, class V>
class BinTree {
   private:
    shared_ptr<Node<K, V>> head;
    shared_ptr<Node<K, V>> min;
    shared_ptr<Node<K, V>> max;

    // Iterations
    class iterator {
       private:
        // TODO: Remove typedef enum state { RIGHT, SELF, END } State;
        // State state;
        shared_ptr<Node<K, V>> curr_node;
        shared_ptr<Node<K, V>> prev_node;
        iterator(shared_ptr<Node<K, V>> node)
            : curr_node(node), prev_node(nullptr){};

        friend class BinTree;

       public:
        K& operator*() const { return *curr_node; };
        iterator& operator++() {  // FIXME: This will not work as is
            if (not this->curr_node) {
                throw NullException("Iterator out of bounds");
            }
            if (this->prev_node or
                this->prev_node == this->curr_node->getRight()) {
                this->prev_node = this->curr_node;
                this->curr_node = this->curr_node->getParent();
            } else if (this->prev_node == this->curr_node->getLeft()) {
                // TODO: Drop right then leftmost
            } else {
                throw NullException("TODO: Idk");
            }
        };
        iterator operator++(int);
        bool operator==(const iterator& it) const;
        bool operator!=(const iterator& it) const;
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;
    };
    iterator begin();
    iterator end();

   public:
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
    iterator begin() const;
    iterator end() const;
    // /**
    //  * @brief Populate `max_array` with the highest `count` values
    //  * @param max_array Allocated array
    //  * @param count The amount to populate
    //  */
    // void fillFromMax(V max_array[], int count) {
    //     throw NullException("Empty Tree");
    // }
    // /**
    //  * @brief Populate `min_array` with the lowest `count` values
    //  * @param max_array Allocated array
    //  * @param count The amount to populate
    //  */
    // void fillFromMin(V min_array[], int count) {
    //     throw NullException("Empty Tree");
    // }
};
}  // namespace LecturesStats