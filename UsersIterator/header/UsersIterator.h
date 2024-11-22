// UsersIterator.h
#ifndef TASK_MANAGEMENT_USERSITERATOR_H
#define TASK_MANAGEMENT_USERSITERATOR_H

#include <vector>
#include "../header/User.h"

class UsersIterator {
private:
    typename std::vector<User>::iterator it;

public:
    explicit UsersIterator(typename std::vector<User>::iterator start) : it(start) {}

    User& operator*() { return *it; }
    User* operator->() { return std::to_address(it); }

    UsersIterator& operator++() {
        ++it;
        return *this;
    }

    bool operator!=(const UsersIterator& other) const {
        return it != other.it;
    }
};

#endif //TASK_MANAGEMENT_USERSITERATOR_H
