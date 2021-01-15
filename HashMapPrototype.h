#ifndef WET_HW2_HASHMAP_PROTO_H
#define WET_HW2_HASHMAP_PROTO_H

#include <memory>
#include <string>

#include "Array.h"

namespace LecturesStats {

using std::shared_ptr;
using std::string;

template <class V>
class HashMap {
    static const int DEFAULT_SIZE = 10;
    static constexpr float MIN_LOAD_FACTOR = 0.16;
    static constexpr float MAX_LOAD_FACTOR = 0.5;
    int size;
    int used;
    class Cell;
    Array<Cell<V>>* data;

    // Calculate hash from key
    int hash(int key);
    // Resize the hashtable to the given size
    void resize(int new_size);

    // Current max capacity
    int max_size();

   public:
    HashMap();
    ~HashMap();

    // Whether the map is empty (used == 0)
    bool isEmpty() const;

    // The load factor (used/size)
    double loadFactor() const;

    /**
     * Removes (and returns) the value mapped to key from the hashmap
     */
    remove(int key);

    /**
     * Checks if the key exist in the map
     * If the key isn't in the map, throws exception
     */
    bool exist(int key) const;

    /**
     * Gets the value mapped to the given key.
     * If the key isn't in the map, throws exception
     */
    const V& get(int key) const;

    /**
     * Set a mapping from the key to the value
     */
    void set(int key, V value);

    /**
     * Returns a reference to the value.
     * If no value exists, creates one with the default c'tor of V (so it can be
     * assigned something else) Use this only to assign or get a value that you
     * know exist
     */
    V& operator[](int key);
};

template <class V>
class HashMap<V>::Cell {
    typedef enum { EMPTY, DELETED, ASSIGNED } CellValueState;

   private:
    CellValueType state;
    V value;

    Cell();

   public:
    /**
     * Sets the value inside the cell.
     * Will override existing value
     */
    void operator=(const V& value);

    /**
     * Sets the value of the current cell to value.
     * If cell already assigned, throws exception instead of overriding
     */
    void set(const V& value);

    /**
     * Get a reference to the value of the current cell.
     * Throws exception if cell is empty or deleted
     */
    V& get();

    /**
     * Set cell state to DELETED.
     * Discards the value of the cell (setting to V() instead to let dtor be called).
     * If not assigned, throws exception
     */
    void empty();

    /**
     * Check if the cell has a value.
     */
    bool hasValue() const;
};

}  // namespace LecturesStats

#endif