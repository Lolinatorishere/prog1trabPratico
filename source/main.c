#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/stringParse.h"
#include "../headers/menu.h"

int login(){
    //todo create a function that searches for the user
    int attempts = 0;
    char buffer[256];
    while(attempts < 3){
        menuPrint("Login", 1, 1);
        printf("Nome:");
        fgets(buffer, 256, stdin);
        printf("Password:");
        fgets(buffer, 256, stdin);
        attempts++;
    }
    return 0;
}

int main(){
    char buffer[256];
    int programState = 0;
    int input;
    while(1){
        switch(programState){
            case 0:
                menuPrint("StartUI", 1, 1);
                printf("input:");
                fgets(buffer, 256, stdin);
                input = int64FromString(buffer);
                if(input <= 0)return 0;
                if(input >= 3)continue;
                programState = input;
                break;
            case 1:
                if(login() == 0){
                    programState = 0;
                }else{
                    programState = 2;
                }
                
                break;
            case 2:
                menuPrint("NewUser", 1, 1);
                break;
            default: 
                return 0;
        }
    }
}
