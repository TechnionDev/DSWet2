#ifndef WET_HW2_HASHMAP_H
#define WET_HW2_HASHMAP_H

#include "HashMapPrototype.h"

namespace LecturesStats {

// Golden ration - 1
const double FACTOR_A = 0.6180339887498948482045868;
// 1/pi
const double FACTOR_B = 0.3183098861837906715377675;

static int hashMul(int factor, int key) {
    double res = factor * key;
    res = res - (long long)res;  // Get only fraction part
    int fin = *((int *)&res);
    return fin;
}

template <class V>
HashMap<V>::Cell::Cell() : state(EMPTY), value(V()) {}

template <class V>
void HashMap<V>::Cell::operator=(const V &value) {
    this->state = ASSIGNED;
    this->value = value;
}

template <class V>
void HashMap<V>::Cell::set(const V &value) {
    if (state == ASSIGNED) {
        throw CellAlreadyAssignedException(
            "Cell::set can only assign to empty cell");
    }
    *this = value;
}

template <class V>
V &HashMap<V>::Cell::get() {
    if (state != ASSIGNED) {
        throw CellEmptyException("Can't get value from an empty cell");
    }
    return this->value;
}

template <class V>
void HashMap<V>::Cell::empty() {
    if (state != ASSIGNED) {
        throw CellEmptyException("Can't empty an empty cell");
    }
    this->state = DELETED;
    this->value = V();
}

template <class V>
bool HashMap<V>::Cell::hasValue() const {
    return state == ASSIGNED;
}

template <class V>
HashMap<V>::HashMap()
    : size(DEFAULT_SIZE), used(0), data(new Array<Cell>(DEFAULT_SIZE)) {}

template <class V>
HashMap<V>::~HashMap() {
    delete data;
}

template <class V>
int HashMap<V>::hash(int key, int hash_count) {
    return key % size;
}

template <class V>
double HashMap<V>::loadFactor() const {
    assert(size != 0);
    return ((double)used) / size;
}

template <class V>
bool HashMap<V>::isEmpty() const {
    return used == 0;
}

template <class V>
V &HashMap<V>::operator[](int key) {
    /* TODO: Actually implement. Currently getCell throws exception if non
     found. Need to create a default to let the user set the value */
    return getCell(key, true).get();
}

template <class V>
void HashMap<V>::resize(int new_size) {
    double load = loadFactor();

    // Verify new size will not cause overload
    assert(new_size / MAX_LOAD_FACTOR >= this->data->used());

    if (DEFAULT_SIZE * (1 / MIN_LOAD_FACTOR) >= size or
        (load >= MIN_LOAD_FACTOR and load <= MAX_LOAD_FACTOR)) {
        // TODO: Debug print warning
        return;
    }
    Array<Cell> *new_data = new Array<Cell>(new_size);
    // TODO: Implement moving all values. Rehash everything and move to new_data
    delete this->data;
    this->data = new_data;
    this->size = new_size;
}

template <class V>
typename HashMap<V>::Cell &HashMap<V>::getCell(int key, bool with_value) {
    const int rehash_limit = max(this->size / 20, 5);
    for (int i = 0, hash = this->hash(key); i < rehash_limit;
         hash = this->hash(key, ++i)) {
        if (with_value) {
            typename Cell::CellValueState state = (*data)[hash].state;
            if (state == Cell::ASSIGNED) {
                // Will update state and copy value
                return (*data)[hash];
            } else if (state == Cell::DELETED) {
                continue;
            } else {  // state == EMPTY
                throw CellEmptyException(
                    "Expected a cell with value but got empty handed");
            }
        } else {
            if (not(*data)[hash].hasValue()) {
                // Will update state and copy value
                return (*data)[hash];
            }
        }
    }
    // TODO: Throw exception
    return (*data)[0];
}

template <class V>
void HashMap<V>::set(int key, const V &value) {
    if (loadFactor() >= MAX_LOAD_FACTOR) {
        this->resize(size * EXPAND_FACTOR);
    }
    Cell &cell = getCell(key, false);
    // TODO: Try catch or something to check if found a cell
    assert(not cell.hasValue());
    cell = value;
    // Couldn't find a good spot even with good load factor
    // TODO: this->resize(size * EXPAND_FACTOR);
    // this->set(key, value);
}

};  // namespace LecturesStats

#endif