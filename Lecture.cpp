#include "Lecture.h"

namespace LecturesStats {
    Lecture::Lecture(int lecture_id, int course_id)
            : lecture_id(lecture_id), course_id(course_id) {}

    int Lecture::get_views() { return views; }

    void Lecture::add_views(int num_views) {
        views += num_views;
    }


}  // namespace LecturesStats