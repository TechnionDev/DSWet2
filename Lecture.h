#ifndef WET_HW1_LECTURE_H
#define WET_HW1_LECTURE_H

namespace LecturesStats {
class Lecture {
    int views = 0;

   public:
    int get_views();
    void add_views(int num_views);
};

}  // namespace LecturesStats
#endif  // WET_HW1_LECTURE_H
