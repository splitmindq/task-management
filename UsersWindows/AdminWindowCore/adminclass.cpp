//
// Created by mzitr on 04.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AdminClass.h" resolved

#include "adminclass.h"
#include "ui_AdminClass.h"


AdminClass::AdminClass(UserManager *userManager,QWidget *parent,const std::string& username) :
                    BasicClass(userManager,parent,username), ui(new Ui::AdminClass) {
    ui->setupUi(this);
//  displayUserInfo();
}

AdminClass::~AdminClass() = default;
