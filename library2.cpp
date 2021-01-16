#include "library2.h"
#include "CoursesManager.h"

void* Init() {
    LecturesStats::CoursesManager* DS = new LecturesStats::CoursesManager();
    return (void*) DS;
}

StatusType AddCourse(void* DS, int courseID) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(((LecturesStats::CoursesManager*) DS)
            ->AddCourse(courseID));
}

StatusType RemoveCourse(void* DS, int courseID) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(
            ((LecturesStats::CoursesManager*) DS)->RemoveCourse(courseID));
}

StatusType AddClass(void* DS, int courseID, int* classID) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(((LecturesStats::CoursesManager*) DS)
            ->AddClass(courseID, classID));
}

StatusType WatchClass(void* DS, int courseID, int classID, int time) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(((LecturesStats::CoursesManager*) DS)
            ->WatchClass(courseID, classID, time));
}

StatusType TimeViewed(void* DS, int courseID, int classID, int* timeViewed) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(
            ((LecturesStats::CoursesManager*) DS)
                    ->TimeViewed(courseID, classID, timeViewed));
}

StatusType GetIthWatchedClass(void* DS, int i, int* courseID, int* classID) {
    if (DS == NULL) {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(
            ((LecturesStats::CoursesManager*) DS)
                    ->GetIthWatchedClass(i, courseID, classID));
}

void Quit(void** DS) {
    delete ((LecturesStats::CoursesManager*) (*DS));
    *DS = nullptr;
}