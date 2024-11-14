//
// Created by mzitr on 14.11.2024.
//

#ifndef TASK_MANAGEMENT_DBEXCEPTIONHANDLER_H
#define TASK_MANAGEMENT_DBEXCEPTIONHANDLER_H


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "dbException.h"
class ExceptionHandler {
public:
    static void handle(const std::exception& e) {
        logError(e.what());

    }
    static void logError(const std::string& message) {
        std::ofstream logFile("error_log.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << "Error: " << message << std::endl;
            logFile.close();
        } else {
            std::cerr << "Ошибка при открытии файла для записи." << std::endl;
        }
    }


    static void throwCustomException(const std::string& message) {
        throw DbException(message);
    }

    void exampleFunction() {
        try {
            ExceptionHandler::throwCustomException("Ошибка при подключении к базе данных");
        } catch (const DbException& e) {
            ExceptionHandler::handle(e);
            std::cerr << "Обработано исключение: " << e.what() << std::endl; // Выводим ошибку в консоль
        }
    }

};


#endif //TASK_MANAGEMENT_DBEXCEPTIONHANDLER_H
