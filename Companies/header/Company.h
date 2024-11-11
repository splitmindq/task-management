//
// Created by mzitr on 04.11.2024.
//

#ifndef TASK_MANAGEMENT_COMPANY_H
#define TASK_MANAGEMENT_COMPANY_H
#include <iostream>
#include <memory>
#include <string>

struct Company{

private:

    int id;
    std::string companyName;
    int adminId;


public:

    Company(int companyId,const std::string &name ,int adminId):
            id(companyId),companyName(name),adminId(adminId){}

    int getCompanyId() const {return id;}
    const std::string &getName() const {return companyName;}
    int getAdminId() const {return adminId;}

    void setAdminId(int id) { adminId = id; }
    void setName(const std::string& name) {companyName = name; }





};


#endif //TASK_MANAGEMENT_COMPANY_H
