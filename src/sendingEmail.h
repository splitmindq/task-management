//
// Created by mzitr on 08.09.2024.
//



#ifndef LABS_SENDINGEMAIL_H
#define LABS_SENDINGEMAIL_H


//dependencies
#include "curl/curl.h"
//-------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <array>
#include <cstring>
#include "locale.h"
#include "user.h"
#include "user_manager.h"
#include "email_sender.h"

//-------------------------------------------------------------------------------------

void display_menu();
void handle_menu_choice(int choice, UserManager& userManager, EmailSender& emailSender);




#endif //LABS_SENDINGEMAIL_H
