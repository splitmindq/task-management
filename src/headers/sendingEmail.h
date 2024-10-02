//
// Created by mzitr on 08.09.2024.
//

#ifndef LABS_SENDING_EMAIL_H
#define LABS_SENDING_EMAIL_H

// Dependencies
#include "curl/curl.h"
//-------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <array>
#include <cstring>
#include "locale.h"
#include "User.h"
#include "userManager.h"
#include "sendingEmail.h"
#include "emailSender.h"

//-------------------------------------------------------------------------------------

void displayMenu();
void handleMenuChoice(int choice, UserManager& userManager, EmailSender& emailSender);

#endif // LABS_SENDING_EMAIL_H