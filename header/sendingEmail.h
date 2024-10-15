//
// Created by mzitr on 08.09.2024.
//

#ifndef LABS_SENDING_EMAIL_H
#define LABS_SENDING_EMAIL_H

// Dependencies

//-------------------------------------------------------------------------------------

#include <iostream>


#include <vector>

#include <string>
#include <memory>
#include <array>
#include <cstring>
#include <locale>
#include "User.h"
#include "UserManager.h"
#include "EmailSender.h"
#include "pqxx/pqxx"


//-------------------------------------------------------------------------------------

void displayMenu();
void handleMenuChoice(int choice, UserManager& userManager, EmailSender& emailSender);

#endif // LABS_SENDING_EMAIL_H