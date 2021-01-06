#include "CourseNode.h"

namespace LecturesStats {
    CourseNode::CourseNode(int course_id)
            : course_id(course_id) {
        shared_ptr<HashMap<Lecture>> Lectures_hash_ptr(new HashMap<Lecture>);
        Lectures_hash_map = Lectures_hash_ptr;
    }

    shared_ptr<Lecture> CourseNode::get_class(int class_num) {
        return Lectures_hash_map->get(class_num);
    }

    void CourseNode::pop_lectures() {
        for (int i = 0; i < numOfClasses; i++) {
            Lectures_hash_map->remove(i);
        }
    }

    int CourseNode::insert_class() {
        shared_ptr<Lecture> Lecture_ptr(new Lecture(numOfClasses, course_id));
        Lectures_hash_map[numOfClasses] = Lecture_ptr;
        numOfClasses++;
        return numOfClasses - 1;
    }
}  // namespace LecturesStats
