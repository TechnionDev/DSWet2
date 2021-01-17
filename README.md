# DSWet2 - Spec Sheet
[![Build Status](https://travis-ci.com/TechnionDev/DSWet2.svg?token=yky5FpFq16PBqRk8YYsp&branch=main)](https://travis-ci.com/TechnionDev/DSWet2)

## General Structure

![](/Users/gtelem/pgit/DSWet2/diagram.png)

The structure of this assignment is mostly based on the structure of the previous assignment.

The CourseManager contains

* 1 `BinTree` to save the order between lectures.
  * Each node is a `Key` (represents a lecture in this case). Comparison according to the defined rules.
* A `HashMap` of courses.
  * Each course contains a `HashMap` or lectures

## Data Structures

There are 3 major data structures used

1. `HashMap<V>`
2. `BinTree<K,V>`
3. `Array<V>` with init time 0.

There are also some minor structures

1. `Key` - Used as comparable key for `BinTree` to keep the data sorted.
2. `CourseNode`
3. `Lecture`

Let's dive deeper into each

### HashMap

This is implemented almost exactly like the course suggest (the double hashing method) with some minos changes.

1. Limit on the amount of rehashing to 10% of the current table max capacity
   1. When limit (or circular hashing) is reaches, we alter the size of the table and try again
2. Circular hashing detection using 0 init `Array`s
   1. Circular hashing is implemented by holding an Array of booleans (as a member of the class) and each time we search for a cell, we reset the `Array` (`top = 0`) and with `O(1)` each time, we check and mark that we didn't get this hash alraedy. This doesn't add to the space complexity because it at most, doubles the size of the structure (most times, much less than double, depencing on the type of value used; `V`)
3. Slightly alter the size of the table, each time we expand or shrink (to avoid exact exponent of 2)

As mentioned, each resize, we use the hash function we already implemented to slightly change the new size for the table (up to 10% different). By doing so, we get both a detirministic approch and at the same time, we greatly lower the chanes for exploding hash table size (it is actually impossible for the size to explode as we always alter the table based on the currently **used** count rather than by a factor or the current capacity).

We checked over 0.3 million `add`s and `remove`s operations with and obeserved linear scaling of runtime.

At times even better than STL's `unordered_map` which performed horrible both when adding a lot of random keys and then removing them (STL's implementation took over 40% longer to run) and when sequencially adding and immedietly removing a lot of keys (STL's took over **twice as long**).

We didn't rely on prime numbers and our tables sizes are much more "erratic" (and depend on the past sized). This, we're more resilient to a cetain set of "bad" values.

Each resize changes the size capacity of the map to `EXPAND_FACTOR*used ±10%` where `EXPAND_FACTOR` is a constant (in our case 8). Each time we remove an element, we make sure the load factor (`used/capacity`) is at least 16th or we resize (btw, this is how we avoid frequent resizing if a used decides to add and remove an element when on the brink of a resize).

Anyways, so the space complexity is  `constant * O(Array)`. Which totals to `O(n)`.

### BinTree (AVL)

Almost no change from previous assignment (or from the course's defined structure).

The major difference is that it in addition to the key, we keep an index and size in the tree for each node. Maintaining it, is very similar to maintaining the size of the tree which was shown in class.

Space is `O(n)` where `n` is the number of key/value pairs in the tree.

### Array with 0 init time

Pretty much the same as was shown in the beginning of the semester. Wee added a clear method to reset the `top` which "sets" all the values back to the default.

The rest is mostly the same.

Space complexity is `O(n)` where `n` is the size (proven in class).

### Lecture/Key

Play similar role (could potentially be merge too). Hold a combination of watch time + courseID + lectureID.

Space is `O(1)` ofc.

### CourseNode

Hold the information on the course. This includes the hash table that holds the lectures as well as the course's id.

Implements some basic function to insert, get and remove lectures.

The space complexity is the space complexity of `HashMap` + `O(1)`. So in total `O(n)` where `n` is the number of lectures for this couse.

## Space/Memory Complexity

So we mentioned the space complexity for each data structure. So the total complexity for the entire system is as follows

`O(courseCount * O(lecturesForThatCourse)) = O(courseCount + `**`total`**`Lectures)` as required.

## Runtime Complexity

So now that we calculated the space complexity (using the known complexities of the structures we implemented), we can get to runtime complexity.

### `Init`

Creates an empty hash table for the courses and an empty AVL tree for the priorities. Both operations are known to be `O(1)` in the worst case.

**Total: `O(1)` worst case**

### `AddCourse`

1. Create empty hash table for the `CourseNode` (worst `O(1)`).
2. Adds a course to the `Courses` hash table (hash table add is amortized average `O(1)`).

**Total: `O(1)` wosrt` + O(1)` amortized average` = O(1)` amortized average**

### `RemoveCourse`

1. For each lecture in the course, remove it from the tree
   1. worst `m` times `O(log(M))` where `m` is the number of of lectures for that course and `M` the total number of lectures. Remove from tree is a known complexity proportioned to the tree size. We already mentioned that each node correlates to a lectures.
   2. Btw, we could've kept a reference for node in the tree in the lectures hash map of each course and reduce the runtime complexity to `O(m)` but it wasn't needed here so yeah.
2. Dealloc hash map of lectures
   1. Only 2 allocations were made. So `O(1)` worst.
3. Remove the course from the courses hash map (amorized average `O(1)`)

### `AddClass`

We need to add an enty to the hash table. Nothing to the tree. So 

**Total: `O(1)` amortized average**

### `WatchClass`

1. Get the `CourseNode` from the hash table - average `O(1)`
2. Get the class from the lectures hash table inside the course - average `O(1)`
3. If the lecture's watch time is 0. Update it and add the lecture to the priority tree - worst `O(log(M))`
   1. If not 0, remove from tree (key with the old watch time) and re-add with the new watch time - worst `O(log(M))`

**Total: `O(log(M))` worst + `O(1)` average = `O(log(M)) average`**

### `TimeViewed`

Same as `WatchClass` but don't need to do anything with the tree. Just pull the lecture and return the `timeViewed`.

**Total:  `O(1)` average**

### `GetIthWatchedClass`

Assuming that the `i`th is a class with a positive (non 0) view time, then we simply search in the tree the node with the index `i` (starting from 1 for some reason) in the tree. We utilize the size of the tree each time we go down one level to see where the index is.

So the total is a search for an element in the AVL tree which is known to be worst `O(log(M))`.

**Total:  `O(log(M))` worst**

### `Quit`

We meed to dealloc the `CoursesManager` object. The only thing we need to dealloc is the tree which dealloc in the time complexity of `O(M)` (recursive iteration on the tree, deallocating each cell and the power of `shared_ptr`s).

In addition, the deallocation of the the courses' hash map. This will deallocate the hash map for each of the courses. Which in turn will deallocate it lecture.

Each lecture appears in exactly one course. So the total number of deallocations it `m+n` because there's at least going to be `n` dealllocations (for the number of courses that can also be with no lectures).

So the total is number of deallocations:

**Total:  `O(m+n)` worst**

