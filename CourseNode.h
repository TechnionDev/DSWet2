#ifndef WET_HW1_COURSENODE_H
#define WET_HW1_COURSENODE_H

#include <memory>

#include "HashMap.h"
#include "Lecture.h"

namespace LecturesStats {

class CourseNode {
   private:
    HashMap<shared_ptr<Lecture>> Lectures_hash_map;
    int numOfClasses = 0;

   public:
    CourseNode() = default;
    shared_ptr<Lecture> get_class(int class_num);
    int insert_class();
    int get_class_num() { return numOfClasses; }
    void pop_lectures();
};
}  // namespace LecturesStats

#endif  // WET_HW1_COURSENODE_H
