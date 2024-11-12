//
// Created by mzitr on 04.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AdminClass.h" resolved

#include "adminclass.h"
#include "ui_AdminClass.h"


AdminClass::AdminClass(UserManager *userManager,QWidget *parent,User *user, std::shared_ptr<Company> company) :
        BasicClass(userManager,parent,user), ui(new Ui::AdminClass),company(company) {
    ui->setupUi(this);

    displayUserInfo();
}

AdminClass::~AdminClass() = default;

void AdminClass::displayUserInfo() {


}