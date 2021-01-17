#ifndef WET_HW2_KEY_H
#define WET_HW2_KEY_H

#include <string>

namespace LecturesStats {
class Key {
   public:
    int views;
    int course_id;
    int lecture_id;
    Key(int views = 0, int course_id = 0, int lecture_id = 0);
};

bool operator>(const Key& key1, const Key& key2);
bool operator>=(const Key& key1, const Key& key2);
bool operator<=(const Key& key1, const Key& key2);
bool operator==(const Key& key1, const Key& key2);
bool operator<(const Key& key1, const Key& key2);
std::string to_string(Key const& key);
}  // namespace LecturesStats

#endif  // WET_HW2_KEY_H
