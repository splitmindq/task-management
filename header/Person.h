//
// Created by mzitr on 17.10.2024.
//

#ifndef TASK_MANAGEMENT_PERSON_H
#define TASK_MANAGEMENT_PERSON_H

class Person {
public:
    virtual void displayInfo() const = 0; // Чисто виртуальная функция
    virtual ~Person() = default; // Виртуальный деструктор
};


#endif //TASK_MANAGEMENT_PERSON_H
