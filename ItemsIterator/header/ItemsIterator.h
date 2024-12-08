// ItemsIterator.h
#ifndef TASK_MANAGEMENT_ITEMSITERATOR_H
#define TASK_MANAGEMENT_ITEMSITERATOR_H

#include <utility>
#include <vector>
#include "../header/User.h"
#include "../Task/header/Task.h"

template <typename T>
class Iterator {
private:
    typename std::vector<T>::iterator it;

public:
    explicit Iterator(typename std::vector<T>::iterator start) : it(std::move(start)) {}

    T &operator*() { return *it; }

    T *operator->() { return std::to_address(it); }

    Iterator &operator++() {
        ++it;
        return *this;
    }

    bool operator!=(const Iterator &other) const {
        return it != other.it;
    }
};

using UsersIterator = Iterator<User>;
using TasksIterator = Iterator<Task>;

#endif //TASK_MANAGEMENT_ITEMSITERATOR_H
