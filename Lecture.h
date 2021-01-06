#ifndef WET_HW1_LECTURE_H
#define WET_HW1_LECTURE_H

namespace LecturesStats {
    class Lecture {
        int views = 0;
        int lecture_id;
        int course_id;
    public:
        Lecture(int lecture_id, int course_id);
        int get_views();
        void add_views(int num_views);
    };

}  // namespace LecturesStats
#endif  // WET_HW1_LECTURE_H
