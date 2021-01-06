#ifndef WET_HW1_COURSENODE_H
#define WET_HW1_COURSENODE_H

#include <memory>
#include "HashMapPrototype.h"
#include "Lecture.h"

namespace LecturesStats {
    using namespace std;

    class CourseNode {
    private:
        shared_ptr<HashMap<Lecture>> Lectures_hash_map;
        int course_id;
        int numOfClasses = 0;

    public:
        CourseNode(int course_id);
        shared_ptr<Lecture> get_class(int class_num);
        int insert_class();
        int get_class_num() { return numOfClasses; }
        void pop_lectures();
    };
}  // namespace LecturesStats

#endif  // WET_HW1_COURSENODE_H
