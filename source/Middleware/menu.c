#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../headers/Middleware/screenPrint.h"
#include "../../headers/Middleware/user.h"
#include "../../headers/Middleware/stringParse.h"
#include "./Menus/middleware.c"
#include "./Menus/userMenus.c"

void login(USERS *user, int *programState){
    //todo create a function that searches for the user
    int attempts = 0;
    char username[256] = {'\0'};
    char password[256] = {'\0'};
    while(attempts < 3){
        menuPrint("Login", 1, 1);
        printf("Nome:");
        fgets(username, 256, stdin);
        if(strlen(username) == 0 || username[0] == '\n'){
            printf("username cant be null\n");
            sleep(1);
            continue;
        }
        for(int i = strlen(username)-1 ; i < 256 ; i++){
            username[i] = '\0';
        }
        printf("Password:");
        fgets(password, 256, stdin);
        if(strlen(password) == 0 || password[0] == '\n'){
            printf("password cant be null\n");
            sleep(1);
            continue;
        }
        for(int i = strlen(password)-1 ; i < 256 ; i++){
            password[i] = '\0';
        }
        if(userValidate(username, password, user) == 0){
            printf("Bem vindo %s\n", user->userName);
            *programState = 3;
            sleep(2);
            return;
        }
        printf("username ou password erradas\n");
        sleep(2);
        attempts++;
    }
    returnText("start Menu", 3);
    *programState = 0;
    return;
}

void startUI(int *programState){
    char buffer[256];
    menuPrint("StartUI", 1, 1);
    printf("input:");
    fgets(buffer, 256, stdin);
    int64_t input = int64FromString(buffer);
    if(input <= 0){*programState = -1; return;}
    if(input >= 3){*programState = -1; return;}
    *programState = input;
}

