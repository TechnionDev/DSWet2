#ifndef WET_HW2_O1_ARRAY_H
#define WET_HW2_O1_ARRAY_H

#include "ArrayPrototype.h"

namespace LecturesStats {

template <class V>
Array<V>::Array(int size, const V& default_value)
    : default_value(default_value),
      indexes(new int[size]),
      shortcuts(new int[size]),
      values(new V[size]),
      size(size),
      top(0) {
    if (size <= 0) {
        delete[] indexes;
        delete[] shortcuts;
        delete[] values;
        throw InsufficientNewSize("Non positive Array size");
    }
}

template <class V>
Array<V>::Array(int new_size, const Array<V>& old)
    : default_value(old.default_value),
      indexes(NULL),
      shortcuts(NULL),
      values(NULL),
      size(new_size),
      top(0) {
    if (new_size <= 0) {
        throw InsufficientNewSize("Non positive Array size");
    }
    if (new_size > top) {
        throw InsufficientNewSize(
            "Insufficient space in new array to contain the old one's values");
    }
    indexes = new int[new_size];
    shortcuts = new int[new_size];
    values = new V[new_size];

    for (auto it : old) {
        (*this)[top] = it;
    }
}

template <class V>
V& Array<V>::operator[](int index) {
    if (index >= size or index < 0)
        throw OutOfBoundsException(
            "Index: " + std::to_string(index) +
            " for array of size: " + std::to_string(size) + ".");

    if (shortcuts[index] >= 0 && shortcuts[index] < top) {
        if (indexes[shortcuts[index]] == index) {
            return values[index];
        }
    }
    values[index] = default_value;
    shortcuts[index] = top;
    indexes[top] = index;
    top++;
    return values[index];
}

template <class V>
V Array<V>::get(int index) const {
    if (index >= size or index < 0)
        throw OutOfBoundsException(
            "Index: " + std::to_string(index) +
            " for array of size: " + std::to_string(size) + ".");

    if (shortcuts[index] >= 0 && shortcuts[index] < top) {
        if (indexes[shortcuts[index]] == index) {
            return values[index];
        }
    }
    return default_value;
}

template <class V>
Array<V>::iterator::iterator(Array<V>& outer, int index)
    : outer(outer), indexes_index(index) {}

template <class V>
typename Array<V>::iterator& Array<V>::iterator::operator++() {
    this->indexes_index++;
    return *this;
}

template <class V>
typename Array<V>::iterator Array<V>::iterator::operator++(int) {
    return iterator(outer, this->indexes_index++);
}

template <class V>
V& Array<V>::iterator::operator*() {
    return outer.values[this->index()];
}

template <class V>
int Array<V>::iterator::index() const {
    return (int)*this;
}

template <class V>
Array<V>::iterator::operator int() const {
    if (indexes_index >= outer.top) {
        throw OutOfBoundsException(
            "Dereferencing out of bound iterator of Array");
    }

    return outer.indexes[indexes_index];
}

template <class V>
bool Array<V>::iterator::operator==(const iterator& it) const {
    return it.indexes_index == indexes_index and &outer == &(it.outer);
}

template <class V>
bool Array<V>::iterator::operator!=(const iterator& it) const {
    return not(*this == it);
}

template <class V>
typename Array<V>::iterator& Array<V>::iterator::operator=(
    const iterator& other) {
    if (&outer != &(other.outer)) {
        throw IncompatibleParentsException(
            "Assignment between iterators of different instances");
    }
    this->indexes_index = other.indexes_index;
    return *this;
}

template <class V>
typename Array<V>::iterator Array<V>::begin() {
    return iterator(*this);
}

template <class V>
typename Array<V>::iterator Array<V>::end() {
    return iterator(*this, top);
}

template <class V>
int Array<V>::length() const {
    return this->size;
}

template <class V>
int Array<V>::used() const {
    return this->top;
}

template <class V>
Array<V>::~Array() {
    delete[] values;
    delete[] indexes;
    delete[] shortcuts;
}

}  // namespace LecturesStats

#endif