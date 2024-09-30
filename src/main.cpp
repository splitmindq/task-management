#include "sendingEmail.h"

int main(){
    SetConsoleOutputCP(CP_UTF8);
    UserManager userManager;
    EmailSender emailSender("<emailsender11@mail.ru>", "smtp://smtp.mail.ru:587",
                            "emailsender11@mail.ru", "XPm2jZ2YS0ECx5q5PKzW", "Kge6zBXU5SFsMnNsSkfk");

   while(true){
       display_menu();
       int choice;
       std::cout<<"Enter u'r choice :";
       std::cin>>choice;
       handle_menu_choice(choice, userManager, emailSender);
   }

   
}