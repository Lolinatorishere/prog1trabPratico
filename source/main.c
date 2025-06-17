#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/Middleware/stringParse.h"
#include "../headers/Middleware/menu.h"
#include "../headers/Middleware/user.h"

void returnText(char *where, int HowLong){
    printf("voltar para %s em %i\n", where, HowLong);
    sleep(1);
    for(int i = 1 ; i < HowLong ; i++){
        printf("%i\n",HowLong - i);
        sleep(1);
    }
    return;
}

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

void NewUser(int *programState){
    char username[256] = {'\0'};
    char password[256] = {'\0'};
    menuPrint("NewUser", 1, 1);
    printf("username:");
    fgets(username, 256, stdin);
    if(strlen(username) == 0 || username[0] == '\n'){
        printf("username cant be null\n");
        sleep(1);
        return;
    }
    for(int i = strlen(username)-1 ; i < 256 ; i++){
        username[i] = '\0';
    }
    printf("password:");
    fgets(password, 256, stdin);
    if(strlen(password) == 0 || password[0] == '\n'){
        printf("password cant be null\n");
        sleep(1);
        return;
    }
    for(int i = strlen(password)-1 ; i < 256 ; i++){
        password[i] = '\0';
    }
    switch(createUser(username,password, 0)){
    case 1:
        printf("\nuser %s criado com sucesso\n", username);
        *programState = 0;
        sleep(1);
        break;
    case 0:
        printf("\n%s ja existe\n", username);
        sleep(1);
        break;
    case -1:
        printf("\nHouve um erro a carregar os utilizadores\n");
        sleep(1);
        break;
    default:
        printf("that was an issue");
        sleep(1);
        return;
    }
    return;
}

void LoggedIn(USERS *user, int *programState){
    char buffer[256];
    if(user->type >= 100){
        menuPrint("Admin", 1, 1);
    }else{
        menuPrint("User", 1, 1);
    }
    printf("input:");
    fgets(buffer, 256, stdin);
    int64_t input = int64FromString(buffer);
    if(user->type >= 100){
        switch(input){
        case 1:
            //administartion of users 
            *programState = 100;
            break;
        case 2:
            //administartion of cursos
            *programState = 200;
            break;
        case 3:
            //administartion of candidaturas
            *programState = 300;
            break;
        case 0:
            *programState = 0;
            printf("Logout de %s, Adeus\n", user->userName);
            returnText("Login Menu", 3);
            *user = setUser();
            break;
        default:
            *programState = 3;
            return;
        }
    }else{
        switch(input){
        case 1:
            //1 - Registar num Curso
            *programState = 10;
            break;
        case 2:
            //2 - Ver Candidaturas
            *programState = 20;
            break;
        case 3:
            //3 - Ver Dados
            *programState = 30;
            break;
        case 0:
            *programState = 0;
            printf("Logout de %s, Adeus\n", user->userName);
            returnText("Login Menu", 3);
            *user = setUser();
            break;
        default:
            *programState = 3;
            return;
        }
    }
    return;
}

void userAdmin(USERS user, int *programState){
    char buffer[256];
    menuPrint("UserControls", 1, 1);
    printf("input:");
    fgets(buffer, 256, stdin);
    int64_t input = int64FromString(buffer);
//1 - Criar Um Novo Utilizador
//2 - Ver Utilizadores registados
//3 - Alterar utilizador registado
//4 - Apagar Utilizador do Sistema
    switch(input){
    case 1:
        *programState = 101;
        return;
    case 2:
        *programState = 102;
        return;
    case 3:
        *programState = 103;
        return;
    case 4:
        *programState = 104;
        return;
    case 5:
        *programState = 105;
        return;
    case 0:
        *programState = 3;
        return;
    default:
        *programState = 100;
        return;
    }
}

void userIndexMenu(int *programState){
    int page = 0;
    char *menuText = NULL;
    char buffer[256] = {'\0'};
    //handle extras
    showAllUsers(&menuText, 5, page);

    advancedPrint(menuText, 1, 1);
    //handle menuInput
    fgets(buffer, 256, stdin);
    *programState = 100;
    if(menuText != NULL) free(menuText);
}

void usernameSearch(int *programState){
    char search[256] = {'\0'};
    char *menuText = NULL;
    int64_t input = 0;
    int page = 0;
    advancedPrint("Pesquizar pelo utilizador", 1, 1);
    printf("username:");
    fgets(search,256, stdin);
    search[strlen(search)] = '\0';
    searchForUsername(&menuText, search, 5, page);
    advancedPrint(menuText, 1, 1);
    if(menuText != NULL) free(menuText);
    fgets(search,256, stdin);
    input = int64FromString(search);
    if(input == 0) *programState = 105;
    return;
}

void idSearch(int *programState){
    char buffer[256] = {'\0'};
    char *menuText = NULL;
    int64_t input = 0;
    int page = 0;
    advancedPrint("Pesquizar pelo utilizador", 1, 1);
    printf("user ID:");
    fgets(buffer,256, stdin);
    input = int64FromString(buffer);
    searchForUserId(&menuText, input, 5, page);
    advancedPrint(menuText, 1, 1);
    if(menuText != NULL) free(menuText);
    fgets(buffer,256, stdin);
    input = int64FromString(buffer);
    if(input == 0) *programState = 105;
    return;
}

void userSearchMenu(int *programState){
    char *menuText = NULL;
    char buffer[256] = {'\0'};
    menuPrint("searchUser", 1, 1);
    //handle menuInput
    fgets(buffer, 256, stdin);
    int64_t input = int64FromString(buffer);
    switch(input){
    //1 - Username
    //2 - Id user
    //3 - Tipo user
    //0 - sair
    case 1:
        *programState = 10501;
        return;
    case 2:
        *programState = 10502;
        return;
    case 3:
        *programState = 10503;
        return;
    default:
        *programState = 100;
        return;
    }
}

int main(){
    int programState = 0;
    int start = 0;
    USERS user;
    if(start == 0){
        user = setUser();
        start = 1;
    }
    while(1){
        switch(programState){
            case 0:
                startUI(&programState);
                continue;
            case 1:
                login(&user, &programState);
                continue;
            case 2:
                NewUser(&programState);
                continue;
            case 3:
                LoggedIn(&user, &programState);
                continue;
            case 10:
                continue;
            case 20:
                continue;
            case 30:
                continue;
                //1 - Administrar Utilizadores
            case 100:
                userAdmin(user, &programState);
                continue;
                //1 - Criar Um Novo Utilizador
                //2 - Ver Utilizadores registados
                //3 - Alterar utilizador registado
                //4 - Apagar Utilizador do Sistema
                //5 - procurar utilizador
            case 101:
                continue;
            case 102:
                userIndexMenu(&programState);
                continue;
            case 103:
                continue;
            case 104:
                continue;
            case 105:// search specific user;
                userSearchMenu(&programState);
                continue;
            case 10501:
                usernameSearch(&programState);
                continue;
            case 10502:
                idSearch(&programState);
                continue;
            continue;
            //2 - Administrar Cursos
            case 200:
                continue;
            //3 - Gerir Candidaturas
            case 300:
                continue;
            default: 
                return 0;
        }
    }
}
