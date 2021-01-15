#ifndef WET_HW2_O1_ARRAY_PROTO_H
#define WET_HW2_O1_ARRAY_PROTO_H

#include <memory>
#include <string>
#include "Exceptions.h"

namespace LecturesStats {
using std::shared_ptr;
using std::string;

template <class V>
class Array {
   private:
    const V default_value;
    int* indexes;
    int* shortcuts;
    V* values;
    const int size;
    int top;

    friend class iterator;

   public:
    // Iterations
    class iterator {
       private:
        Array& outer;
        int indexes_index;
        iterator(Array& outer, int index = 0);
        friend class Array<V>;

       public:
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&);
        iterator& operator++();
        iterator operator++(int);
        V& operator*();
        int index() const;
        bool operator==(const iterator& it) const;
        bool operator!=(const iterator& it) const;
        operator int() const;
    };

    Array(int size, const V& default_value = V());
    Array(int new_size, const Array<V>& old);
    ~Array();
    /**
     * Sets this[index] to the default and return a reference to it
     */
    V& operator[](int index);

    /**
     * Get the value in this[index] without setting to default if not exist. Returns a copy.
     * Will retain the current used size
     */
    V get(int index) const;
    iterator begin();
    iterator end();
    int length() const;
    int used() const;
};

}  // namespace LecturesStats

#endif