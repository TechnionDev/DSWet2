#include "Lecture.h"

namespace LecturesStats {

int Lecture::get_views() { return views; }

void Lecture::add_views(int num_views) { views += num_views; }

}  // namespace LecturesStats