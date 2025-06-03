#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/structs.h"
#define dir "./data/userData.dat"

USERS setUser(){
    USERS user;
    for(int i = 0 ; i < 256 ; i++){
        user.userName[i] = '\0';
        user.password[i] = '\0';
    }
    user.type = -1;
    user.userId = -1;
    return user;
}

USERS *searchUsername(USERS **userList, int64_t listSize , char *username, int64_t *index) {
    for(int64_t i = 0 ; i < listSize ; i++){
        if(strcmp(userList[i]->userName, username) == 0){
            *index = i;
            return userList[i];
        }
    }
    return NULL;
}

USERS *searchId(USERS **userList, int64_t listSize, int id, int64_t *index) {
    for(int64_t i = 0 ; i < listSize ; i++){
        if(userList[i]->userId == id){
            *index = i;
            return userList[i];
        }
    }
    return NULL;
}

int updateUserData(USERS *user, USERS *userList, int64_t listSize){
    int64_t size = listSize;
    FILE *fp = fopen(dir, "wb");
    if(!fp)
        return -1;
    if(&user != NULL)
        size+=1;
    fwrite(&size, sizeof(int64_t), 1, fp);
    fwrite(userList, sizeof(USERS), listSize, fp);
    if(&user != NULL)
        fwrite(user, sizeof(USERS), 1, fp);
    fclose(fp);
}

int loadUserData(USERS **userList, int64_t *userTotal) {
    int64_t filesize = 0;
    USERS user;
    user = setUser();
    FILE *fp = fopen(dir, "rb");
    if(!fp){
        FILE *makeFile = fopen(dir, "wb");
        fclose(makeFile);
        fp = fopen(dir, "rb");
        if(!fp)
            return -1;
    }
    fseek(fp, 0, SEEK_END);
    if(ftell(fp) == 0){
        fclose(fp);
        return 1;
    }
    fseek(fp, 0, SEEK_SET);
    fread(userTotal, sizeof(int64_t), 1, fp);
    if(userTotal == 0)
        *userTotal = 1;
    *userList = (USERS *)malloc(sizeof(USERS) * *userTotal);
    if(*userList == NULL){ 
        fclose(fp);
        return -1;
    }
    for(int i = 0 ; i < *userTotal ; i++){
        fread(&user, sizeof(USERS), 1, fp);
        strcpy(userList[i]->userName, user.userName);
        strcpy(userList[i]->password, user.password);
        userList[i]->type = user.type;
        userList[i]->userId = user.userId;
    }
    fclose(fp);
    return 0;
}

void freeUserData(USERS *userList) {
    if(userList == NULL)
        return;
    free(userList);
}

int createUser(char *username, char *password, int type){
    int64_t listSize = 0;
    int64_t index = 0;
    USERS *users = NULL;
    USERS user;
    user = setUser();
    switch(loadUserData(&users, &listSize)){
    case -1:
        return -1;
    case 1:
        strcpy(user.userName, username);
        strcpy(user.password, password);
        user.type = 100;
        user.userId = 1;
        users = (USERS *)malloc(sizeof(USERS));
        updateUserData(&user, users, 0);
        free(users);
        return 1;
    default:
        USERS *check = NULL;
        check = searchUsername(&users, listSize, username, &index);
        if(check != NULL){
            return 1;
        }
        strcpy(user.userName, username);
        strcpy(user.password, password);
        user.type = type;
        user.userId = users[listSize].userId + 1;
        updateUserData(&user, users, listSize + 1);
        freeUserData(users);
        break;
    }
}

int updateUser(int id, char *username, char *password, int type){
    int64_t userTotal = 0;
    int64_t index = 0;
    USERS *users = NULL;
    USERS *user = NULL;
    switch(loadUserData(&users, &userTotal)){
        case -1:
            return -1;
        case 1:
            return 1;
        default:
            user = searchId(&users, userTotal, id, &index);
            if(!user)return 2;
            if(username != NULL)
                strcpy(users[index].userName, username);
            if(password != NULL)
                strcpy(users[index].password, password);
            if(type > 0)
                users[index].type = type;
            updateUserData(NULL, users, userTotal);
            freeUserData(users);
            return 0;
        }
}

int deleteUser(int id){
    int64_t listSize = 0;
    int64_t index = 0;
    USERS *users = NULL;
    USERS *user = NULL;
    switch(loadUserData(&users, &listSize)){
        case -1:
            return -1;
        case 1:
            return 1;
        default:
            user = searchId(&users, listSize, id, &index);
            if(!user)return 0;
            // aray[x] replaced with [x+n];
            // because i dont know how to do it better kek;
            if(index != listSize-1){
                for(int i = index ; i < listSize-1 ; i++){
                    users[i] = users[i+1];
                }
            }
            updateUserData(NULL, users, listSize-1);
            freeUserData(users);
            return 0;
    }
}

int userValidate(char *username,char *password, USERS *user){
    USERS *users = NULL;
    int64_t userTotal = 0;
    int checks = 0;
    *user = setUser();
    switch(loadUserData(&users, &userTotal)){
        case -1:
            return 1;
        case 1:
            return 1;
        default:
            for(int i = 0 ; i < userTotal ; i++){
                checks = 0;
                if(strcmp(users[i].userName, username) == 0){
                    checks++;
                }
                if(strcmp(users[i].password, password) == 0){
                    checks++;
                }
                if(checks == 2){
                    strcpy(user->userName, users[i].userName);
                    strcpy(user->password, users[i].password);
                    user->type = users[i].type;
                    user->userId = users[i].userId;
                    break;
                }
            }
            freeUserData(users);
            return 0;
    }
}
