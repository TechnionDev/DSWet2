#include "Key.h"

namespace LecturesStats {
    Key::Key(int views, int course_id, int lecture_id) :
            views(views), course_id(course_id), lecture_id(lecture_id) {}

    bool operator==(const Key& key1, const Key& key2) {
        return key1.views == key2.views and key1.course_id == key2.course_id and key1.lecture_id == key2.lecture_id;
    }

    bool operator<(const Key& key1, const Key& key2) {
        if (key1.views < key2.views) {
            return true;
        } else if (key1.views == key2.views) {
            if (key1.course_id > key2.course_id) {
                return true;
            } else if (key1.course_id == key2.course_id) {
                if (key1.lecture_id > key2.lecture_id) {
                    return true;
                }
            }
        }
        return false;
    }

    bool operator>(const Key& key1, const Key& key2) {
        if (key1.views > key2.views) {
            return true;
        } else if (key1.views == key2.views) {
            if (key1.course_id < key2.course_id) {
                return true;
            } else if (key1.course_id == key2.course_id) {
                if (key1.lecture_id < key2.lecture_id) {
                    return true;
                }
            }
        }
        return false;
    }

    bool operator<=(const Key& key1, const Key& key2) {
        return key1 == key2 or key1 < key2 ;
    }

    bool operator>=(const Key& key1, const Key& key2) {
        return key1 == key2 or key1 > key2;
    }

    std::string to_string(const Key& key) {
        return std::to_string(key.views) + "_" + std::to_string(key.course_id) + "_" + std::to_string(key.lecture_id);
    }
}  // namespace LecturesStats