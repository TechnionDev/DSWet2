#ifndef WET_HW1_HASHMAP_PROTO_H
#define WET_HW1_HASHMAP_PROTO_H

#include <memory>
#include <string>

namespace LecturesStats {

using std::shared_ptr;
using std::string;

/**
 * Represents a temporary cell handle
 */
template <class V>
class Cell {
   private:
    shared_ptr<V> value;

    Cell(shared_ptr<V> value);
    friend HashMap<V>;

   public:
    /**
     * Sets the value inside the cell.
     * Generally, after using this function, the object has no longer a purpose
     */
    void operator=(V& value);

    /**
     * Access an element inside the cell (assuming V has operator[] defined)
     */
    auto operator[](int key);  // Something like {return (*value)[key]}
};

template <class V>
class HashMap {
   public:
    HashMap();
    ~HashMap();

    // You must be stupid if you need docstring for these two
    unsigned int size();
    bool isEmpty() const;

    /**
     * Removes (and returns) the value mapped to key from the hashmap
     */
    remove(int key);

    /**
     * Gets the value mapped to the given key
     */
    shared_ptr<V> get(int key) const;

    /**
     * Returns an assignable cell reference.
     * This means that it isn't safe to keep the return value into a reference
     * variable
     */
    Cell<V> operator[](int key);

    /**
     * Return a readonly cell reference
     */
    const Cell<V> operator[](int key) const;
};

}  // namespace LecturesStats

#endif