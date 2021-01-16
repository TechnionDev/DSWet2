#ifndef WET_HW2_EXCEPTIONS_H
#define WET_HW2_EXCEPTIONS_H

#include <string>

namespace LecturesStats {

#define EXCEPTION(name)                      \
    class name : public Exception {          \
       public:                               \
        name(string str) : Exception(str){}; \
    };

using std::string;

class Exception : public std::runtime_error {
   public:
    // Exception() : std::runtime_error(){};
    Exception(string str) : std::runtime_error(str){};
};

EXCEPTION(OutOfBoundsException);
EXCEPTION(IncompatibleParentsException);
EXCEPTION(InsufficientNewSize);
EXCEPTION(AlreadyExistException);
EXCEPTION(CellEmptyException);
EXCEPTION(CellAlreadyAssignedException);
EXCEPTION(RehashLimitExceededException);
EXCEPTION(ValueNotFoundException);

}  // namespace LecturesStats

#endif