//
// Created by mzitr on 17.11.2024.
//

#ifndef TASK_MANAGEMENT_XMLHANDLER_H
#define TASK_MANAGEMENT_XMLHANDLER_H
#include "iostream"
#include "string"
#include "vector"
#include "tinyxml2.h"
#include "User.h"
#include "../Companies/header/CompanyManager.h"
class XmlHandler {
public:
    XmlHandler() = default;

    // Создание файла XML (выполняется только один раз)
    void createXmlFile(const std::string &filename, const std::string &rootElementName) {
        if (std::filesystem::exists(filename)) {
            std::cout << "Файл уже существует: " << filename << std::endl;
            return;
        }

        tinyxml2::XMLDocument document;
        tinyxml2::XMLElement *root = document.NewElement(rootElementName.c_str());
        document.InsertFirstChild(root);

        if (document.SaveFile(filename.c_str()) == tinyxml2::XML_SUCCESS) {
            std::cout << "XML файл успешно создан: " << filename << std::endl;
        } else {
            std::cerr << "Ошибка при создании XML файла!" << std::endl;
        }
    }

    void addUserToXml(const std::string &filename, User *user) {
        std::string companyName;
        try {
            pqxx::connection C("host=localhost dbname=database user=mzitr password=yourpassword");
            pqxx::work txn(C);
            std::string query = "SELECT name FROM companies WHERE adminId = " + txn.quote(user->id) + " LIMIT 1;";
            pqxx::result result = txn.exec(query);

            if (!result.empty()) {
                companyName = result[0][0].as<std::string>();
            }

            txn.commit();
        } catch (const std::exception &e) {
            std::cerr << "Error finding company: " << e.what() << std::endl;
        }

        tinyxml2::XMLDocument document;
        if (document.LoadFile(filename.c_str()) == tinyxml2::XML_SUCCESS) {
            tinyxml2::XMLElement *root = document.FirstChildElement();
            if (root) {
                tinyxml2::XMLElement *userElement = document.NewElement("User");
                userElement->SetAttribute("name", user->name.c_str());
                userElement->SetAttribute("surname", user->surname.c_str());
                userElement->SetAttribute("email", user->email.c_str());
                userElement->SetAttribute("role", user->role.c_str());
                userElement->SetAttribute("company", companyName.c_str());
                root->InsertEndChild(userElement);

                if (document.SaveFile(filename.c_str()) == tinyxml2::XML_SUCCESS) {
                    std::cout << "Пользователь успешно добавлен в XML!" << std::endl;
                } else {
                    std::cerr << "Ошибка при сохранении XML файла!" << std::endl;
                }
            } else {
                std::cerr << "Ошибка: корневой элемент не найден!" << std::endl;
            }
        } else {
            std::cerr << "Ошибка при загрузке XML файла!" << std::endl;
        }
    }

    void clearRootContent(const std::string &filename) {
        tinyxml2::XMLDocument document;

        if (document.LoadFile(filename.c_str()) == tinyxml2::XML_SUCCESS) {
            tinyxml2::XMLElement *root = document.FirstChildElement();
            if (root) {
                while (root->FirstChild()) {
                    root->DeleteChild(root->FirstChild());
                }

                if (document.SaveFile(filename.c_str()) == tinyxml2::XML_SUCCESS) {
                    std::cout << "Содержимое корневого элемента очищено: " << filename << std::endl;
                } else {
                    std::cerr << "Ошибка при сохранении XML файла!" << std::endl;
                }
            } else {
                std::cerr << "Корневой элемент не найден в XML файле!" << std::endl;
            }
        } else {
            std::cerr << "Ошибка при загрузке XML файла!" << std::endl;
        }
    }

};
#endif