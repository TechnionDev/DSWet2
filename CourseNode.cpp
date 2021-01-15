#include "CourseNode.h"

namespace LecturesStats {
    CourseNode::CourseNode(int course_id)
            : course_id(course_id) {
    }

    shared_ptr<Lecture> CourseNode::get_class(int class_num) {
        assert(Lectures_hash_map.exist(class_num));//we already check if the class is in the range of the array
        return Lectures_hash_map.get(class_num);
    }

    void CourseNode::pop_lectures() {
        for (int i = 0; i < numOfClasses; i++) {
            Lectures_hash_map.remove(i);
        }
    }

    int CourseNode::insert_class() {
        shared_ptr<Lecture> Lecture_ptr(new Lecture(numOfClasses, course_id));
        assert(Lectures_hash_map.exist(numOfClasses));//we already check that the class isn't in the array
        Lectures_hash_map.set(numOfClasses, Lecture_ptr);
        numOfClasses++;
        return numOfClasses - 1;
    }
}  // namespace LecturesStats
