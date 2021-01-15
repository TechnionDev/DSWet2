#ifndef WET_HW1_BINTREE_PROTO_H
#define WET_HW1_BINTREE_PROTO_H

#include <iostream>
#include <memory>
#include <string>
#include "Exceptions.h"

#define TREE_PRINT_SPREAD 10

namespace LecturesStats {
using std::ostream;
using std::shared_ptr;
using std::string;


template <class K, class V>
class BinTree;

template <class K, class V>
class Node {
   private:
    K key;
    shared_ptr<V> value;
    int height;
    int size;
    Node<K, V>* parent;
    Node<K, V>* left;
    Node<K, V>* right;

    void setParent(Node<K, V>* parent);
    short balance();
    friend class BinTree<K, V>;

   public:
    Node(const K& key, shared_ptr<V> value, Node<K, V>* left = NULL,
         Node<K, V>* right = NULL);
    void setLeft(Node<K, V>* node);
    void setRight(Node<K, V>* node);
    bool isLeaf();
    Node<K, V>* getParent();
    Node<K, V>* getLeft();
    Node<K, V>* getRight();
    shared_ptr<V> getValue();
    const K& getKey();
    const int getSize();
#ifndef NDEBUG
    static void print2DUtil(ostream& os, Node<K, V>* root, int space);
#endif
};

template <class K, class V>
class BinTree {
   private:
    Node<K, V>* head;
    Node<K, V>* min_node = NULL;
    Node<K, V>* max_node = NULL;

    void rotateLL(Node<K, V>*(&node));
    void rotateLR(Node<K, V>*(&node));
    void rotateRR(Node<K, V>*(&node));
    void rotateRL(Node<K, V>*(&node));
    Node<K, V>* find(const K& key);
    Node<K, V>* findN(int index);
    Node<K, V>* fromRange(int low, int high);

   public:
#ifndef NDEBUG
    // Validates the tree's structure. Makes sure (recursively) that everything
    // points where it should. For debug asserts
    bool isTreeStructured();
    static bool isTreeStructured(Node<K, V>* parent, Node<K, V>* node);
#endif
    // Iterations
    class iterator {
       private:
        Node<K, V>* curr;
        Node<K, V>* prev;
        iterator(Node<K, V>* node);
        void rise();
        void dropLeft();

        friend class BinTree<K, V>;

       public:
        iterator& operator++();
        iterator operator++(int);
        const shared_ptr<V> value() const;
        const K& key() const;
        bool operator==(const iterator& it) const;
        bool operator!=(const iterator& it) const;
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;
    };
    iterator begin() const;
    iterator end() const;
    BinTree();
    BinTree(int size);
    bool isEmpty();
    ~BinTree();
    void deallocTree(Node<K, V>* curr);
    void updateSizeUp(Node<K, V>* curr);
    shared_ptr<K> getMaxKey();
    shared_ptr<V> getMax();
    int sizeOfTree();


    /**
     * @brief Get the value attached to the given key
     * @param key Key
     * @return shared_ptr<V> Value
     */
    shared_ptr<V> get(const K& key);
    shared_ptr<V> getN(int index);
    /**
     * @brief Same as get but removes the element from the tree
     * @param key Key
     * @return shared_ptr<V> Value
     */
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
    void print();
#endif
};
}  // namespace LecturesStats
#endif // WET_HW1_BINTREE_PROTO_H