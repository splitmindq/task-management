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

    void createXmlFile(const std::string &filename, const std::string &rootElementName) {
        if (std::filesystem::exists(filename)) {
            std::cout << "File already exists: " << filename << std::endl;
            return;
        }

        tinyxml2::XMLDocument document;
        tinyxml2::XMLElement *root = document.NewElement(rootElementName.c_str());
        document.InsertFirstChild(root);

        if (document.SaveFile(filename.c_str()) == tinyxml2::XML_SUCCESS) {
            std::cout << "XML file created successfully: " << filename << std::endl;
        } else {
            std::cerr << "Error creating XML file!" << std::endl;
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
                    std::cout << "User added to XML successfully!" << std::endl;
                } else {
                    std::cerr << "Error saving XML file!" << std::endl;
                }
            } else {
                std::cerr << "Error: root element not found!" << std::endl;
            }
        } else {
            std::cerr << "Error loading XML file!" << std::endl;
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
                    std::cout << "Root content cleared: " << filename << std::endl;
                } else {
                    std::cerr << "Error saving XML file!" << std::endl;
                }
            } else {
                std::cerr << "Root element not found in XML file!" << std::endl;
            }
        } else {
            std::cerr << "Error loading XML file!" << std::endl;
        }
    }
};

#endif