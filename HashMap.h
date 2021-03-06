#ifndef WET_HW2_HASHMAP_H
#define WET_HW2_HASHMAP_H

#include <cassert>

#include "HashMapPrototype.h"

namespace LecturesStats {

// Golden ration - 1
const double FACTOR_A = 0.6180339887498948482045868;
// 1/pi
const double FACTOR_B = 0.3183098861837906715377675;
const int SMALL_PRIME = 7;

static inline double hashMul(double factor, int key) {
    double res = factor * key;
    res = res - (long long)res;  // Get only fraction part
    // int fin = *((int *)&res);
    return res;
}

template <class V>
HashMap<V>::Cell::Cell() : state(EMPTY), value(V()) {}

template <class V>
void HashMap<V>::Cell::set(int key, const V &value) {
    if (state == ASSIGNED) {
        throw CellAlreadyAssignedException(
            "Cell::set can only assign to empty cell");
    }
    this->state = ASSIGNED;
    this->value = value;
    this->key = key;
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
    : size(DEFAULT_SIZE),
      used(0),
      data(new Array<Cell>(DEFAULT_SIZE)),
      visited_hashes(new Array<bool>(DEFAULT_SIZE)) {}

template <class V>
HashMap<V>::~HashMap() {
    delete data;
    delete visited_hashes;
}

template <class V>
int HashMap<V>::hash(int key, int hash_count) {
    double hash1 = (hashMul(FACTOR_A, key)) * size;
    double hash2 = (key % (size - 2)) + 1;
    // Verify hash in range
    assert((int)hash1 >= 0);
    assert((int)hash1 < size);
    assert((int)hash2 >= 1);
    assert((int)hash2 < size - 1);
    int res = std::abs((int)(hash1 + hash2 * hash_count) % size);
    return res;
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
    Cell *cell_ptr = &getCell(key, true);

    if (cell_ptr == &no_cell) {
        // Set to default
        this->set(key, V());
        cell_ptr = &getCell(key, true);
    }
    // Return reference to value
    return cell_ptr->value;
}

template <class V>
const V &HashMap<V>::get(int key) {
    Cell &cell = getCell(key, true);
    if (&cell == &no_cell) {
        throw ValueNotFoundException("Value not found for key " +
                                     to_string(key));
    }
    return cell.get();
}

template <class V>
void HashMap<V>::resize(int new_size) {
    new_size = max(new_size, (const int)HashMap<V>::DEFAULT_SIZE);

    // Verify new size will not cause overload
    assert(this->used / new_size < MAX_LOAD_FACTOR);

    int diff = (int)(hashMul(FACTOR_A, this->size) * (new_size / 10));
    // At least change by 1
    diff = max(1, diff);

    if (DEFAULT_SIZE < this->size and
        loadFactor() < (MIN_LOAD_FACTOR + MAX_LOAD_FACTOR) / 2) {
        diff *= -1;
    }

    new_size = new_size + diff;
    delete this->visited_hashes;
    this->visited_hashes = new Array<bool>(new_size);
    Array<Cell> *new_data = new Array<Cell>(new_size);
    Array<Cell> *old_data = this->data;

#ifndef NDEBUG
    resize_count++;
    std::cout << "Resizing from " << this->size << " new size " << new_size
              << ". Moving " << old_data->used() << ". New load "
              << (this->used / new_size) << std::endl;
#endif

    this->data = new_data;
    this->used = 0;
    this->size = new_size;

    bool old_resizing = this->resizing;
    this->resizing = true;
    for (auto cell : *old_data) {
        if (cell.hasValue()) {
            this->set(cell.key, cell.value);
        }
    }
    this->resizing = old_resizing;

    delete old_data;
}

template <class V>
typename HashMap<V>::Cell &HashMap<V>::getCell(int key, bool with_value) {
    const int rehash_limit = max(this->size / 10, 10);
    Array<bool> &visited_hashes = *this->visited_hashes;
    visited_hashes.clear();
    for (int i = 0, hash = this->hash(key); i < rehash_limit;
         hash = this->hash(key, ++i)) {
        if (visited_hashes[hash] == true) {
            // Update size to we might get lucky next time
            this->resize(this->used * EXPAND_FACTOR);
            return getCell(key, with_value);
        }
        visited_hashes[hash] = true;
        Cell &cell = (*data)[hash];
        if (with_value) {
            if (cell.state == Cell::ASSIGNED) {
                if (cell.key != key) {
                    continue;  // Hash collision.
                }
                // Will update state and copy value
                return cell;
            } else if (cell.state == Cell::DELETED) {
                continue;
            } else {  // state == EMPTY
                // Return a cell that represents that the value wasn't found
                return no_cell;
            }
        } else {
            if (not cell.hasValue()) {
                // Will update state and copy value
                return cell;
            }
        }
    }
    this->resize(this->used * EXPAND_FACTOR);

    return getCell(key, with_value);
}

template <class V>
void HashMap<V>::set(int key, const V &value) {
    Cell &cell = getCell(key, false);
    // The above can't fail. It will expand, shrink, dance naked, sell a kidney,
    // jump off the roof and basically do whatever is needed to get an empty
    // cell
    assert(not cell.hasValue());
    assert(&cell != &no_cell);
    cell.set(key, value);
    this->used++;
    double load = loadFactor();
    if (load >= MAX_LOAD_FACTOR) {
        this->resize(used * EXPAND_FACTOR);
    }
    load = loadFactor();
    assert(this->resizing or
           (load < MAX_LOAD_FACTOR &&
            (load > MIN_LOAD_FACTOR or size <= DEFAULT_SIZE * EXPAND_FACTOR)));
}

template <class V>
void HashMap<V>::remove(int key) {
    assert(this->resizing == false);
    Cell &cell = getCell(key, true);
    if (&cell == &no_cell) {
        throw ValueNotFoundException("The key " + to_string(key) +
                                     " is not in the dictionary");
    }
    assert(cell.key == key);
    cell.empty();
    this->used--;
    double load = loadFactor();
    if (this->size > DEFAULT_SIZE * EXPAND_FACTOR and load < MIN_LOAD_FACTOR) {
        this->resize(used * EXPAND_FACTOR);
    }
    load = loadFactor();
    assert(this->resizing == false);
#ifndef NDEBUG
    if (not(load <= MAX_LOAD_FACTOR &&
            (load >= MIN_LOAD_FACTOR or
             size <= DEFAULT_SIZE * EXPAND_FACTOR))) {
        std::cout << "Bad load: " << loadFactor() << std::endl;
    }
#endif
    assert((load <= MAX_LOAD_FACTOR &&
            (load >= MIN_LOAD_FACTOR or size <= DEFAULT_SIZE * EXPAND_FACTOR)));
}

template <class V>
bool HashMap<V>::exist(int key) {
    Cell &cell = getCell(key, true);
    if (&cell == &no_cell) {
        return false;
    }

    assert(cell.key == key);
    return true;
}

};  // namespace LecturesStats

#endif