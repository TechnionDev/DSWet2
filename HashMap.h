#ifndef WET_HW2_HASHMAP_H
#define WET_HW2_HASHMAP_H

#include "HashMapPrototype.h"

namespace LecturesStats {

// Golden ration - 1
const double FACTOR_A = 0.6180339887498948482045868;
// 1/pi
const double FACTOR_B = 0.3183098861837906715377675;
const int SMALL_PRIME = 7;

static double hashMul(double factor, int key) {
    double res = factor * key;
    res = res - (long long)res;  // Get only fraction part
    // int fin = *((int *)&res);
    return res;
}

template <class V>
HashMap<V>::Cell::Cell() : state(EMPTY), value(V()) {}

// template <class V> TODO: Remove
// void HashMap<V>::Cell::operator=(const V &value) {
//     this->state = ASSIGNED;
//     this->value = value;
// }

template <class V>
void HashMap<V>::Cell::set(int key, const V &value) {
    if (state == ASSIGNED) {
        throw CellAlreadyAssignedException(
            "Cell::set can only assign to empty cell");
    }
    this->state = ASSIGNED;
    this->value = value;
    this->key = key;
    // *this = value; TODO: Remove
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
    double hash1 = (hashMul(FACTOR_A, key)) * size;
    double hash2 = (key % (size - 2)) + 1;
    // Verify hash in range
    assert((int)hash1 >= 0);
    assert((int)hash1 < size);
    assert((int)hash2 >= 1);
    assert((int)hash2 < size - 1);

    return (int)(hash1 + hash2 * hash_count) % size;
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
    /* TODO: Actually implement. Currently getCell throws exception if non
     found. Need to create a default to let the user set the value */
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
    if (new_size == size) return;
    double load = loadFactor();

    // Verify new size will not cause overload
    assert(this->used / new_size < MAX_LOAD_FACTOR);

    // if (DEFAULT_SIZE * (1 / MIN_LOAD_FACTOR) > new_size or
    //     (load >= MIN_LOAD_FACTOR and load <= MAX_LOAD_FACTOR)) {
    //     // TODO: Debug print warning
    //     return;
    // }
    Array<Cell> *new_data = new Array<Cell>(new_size);
    Array<Cell> *old_data = this->data;
    this->data = new_data;
    this->used = 0;
    this->size = new_size;

    this->resizing = true;
    for (auto cell : *old_data) {
        if (cell.hasValue()) {
            this->set(cell.key, cell.value);
        }
    }
    this->resizing = false;

    delete old_data;
}

template <class V>
typename HashMap<V>::Cell &HashMap<V>::getCell(int key, bool with_value) {
    const int rehash_limit = max(this->size / 20, 10);
    for (int i = 0, hash = this->hash(key); i < rehash_limit;
         hash = this->hash(key, ++i)) {
        Cell &cell = (*data)[hash];
        if (with_value) {
            if (cell.state == Cell::ASSIGNED) {
                if (cell.key != key) {
                    continue;  // Hash collision. // TODO: Maybe debug counting
                               // collissions
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
    // TODO: Throw exception and print warning
    throw RehashLimitExceededException(
        "Can't locate cell within rehashing limit");
}

template <class V>
void HashMap<V>::set(int key, const V &value) {
    Cell &cell = getCell(key, false);
    // TODO: Try catch or something to check if found a cell
    assert(not cell.hasValue());
    cell.set(key, value);
    this->used++;
    // Couldn't find a good spot even with good load factor
    // TODO: this->resize(size * EXPAND_FACTOR);
    // this->set(key, value);
    double load = loadFactor();
    if (load >= MAX_LOAD_FACTOR) {
        this->resize(used * EXPAND_FACTOR);
    }
    load = loadFactor();
    assert(this->resizing or
           (load < MAX_LOAD_FACTOR &&
            (load > MIN_LOAD_FACTOR or size <= DEFAULT_SIZE)));
}

template <class V>
void HashMap<V>::remove(int key) {
    Cell &cell = getCell(key, true);
    // TODO: Check if cell was found
    assert(cell.key == key);
    cell.empty();
    this->used--;
    double load = loadFactor();
    if (load < MIN_LOAD_FACTOR) {
        this->resize(used * EXPAND_FACTOR);
    }
    load = loadFactor();
    assert(this->resizing or
           (load < MAX_LOAD_FACTOR &&
            (load > MIN_LOAD_FACTOR or size <= DEFAULT_SIZE)));
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