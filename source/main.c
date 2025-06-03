#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/stringParse.h"
#include "../headers/menu.h"
#include "../headers/userMiddleware/user.h"

int login(USERS **user){
    //todo create a function that searches for the user
    int attempts = 0;
    char username[256];
    char password[256];
    while(attempts < 3){
        menuPrint("Login", 2, 1);
        printf("Nome:");
        fgets(username, 256, stdin);
        printf("Password:");
        fgets(password, 256, stdin);
        *user = userValidate(username, password);
        if(!user){
            printf("username ou password erradas\n");
            sleep(2);
            attempts++;
        }
    }
    printf("voltar para start Menu\n");
    sleep(1);
    return 0;
}

int main(){
    char buffer[256];
    int programState = 0;
    int input;
    USERS *user = NULL;
    while(1){
        switch(programState){
            case 0:
                menuPrint("StartUI", 1, 2);
                printf("input:");
                fgets(buffer, 256, stdin);
                input = int64FromString(buffer);
                if(input <= 0)return 0;
                if(input >= 3)continue;
                programState = input;
                break;
            case 1:
                if(login(&user) == 0){
                    programState = 0;
                }else{
                    programState = 3;
                }
                break;
            case 2:
                char *username = (char*) malloc(sizeof(char)*256);
                char *password = (char*) malloc(sizeof(char)*256);
                menuPrint("NewUser", 1, 1);
                printf("username:");
                fgets(username, 256, stdin);
                username[strlen(username)] = '\0';
                printf("password");
                fgets(password, 256, stdin);
                password[strlen(username)] = '\0';
                switch(createUser(username,password, 0)){
                case 1:
                    printf("\nuser %s criado com sucesso\n", username);
                    programState = 0;
                    sleep(1);
                    break;
                case -1:
                    printf("\n%s ja existe\n", username);
                    sleep(1);
                    break;
                case 0:
                    printf("\nHouve um erro a carregar os utilizadores\n");
                    sleep(1);
                    break;
                default:
                    free(username);
                    free(password);
                    printf("that was an issue");
                    sleep(1);
                    return -1;
                }
                free(username);
                free(password);
                programState = 0;
                break;
            default: 
                return 0;
        }
    }
}
