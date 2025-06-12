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

void copyUser(USERS *dst, USERS src){
    for(int i = 0 ; i < strlen(src.userName) ; i++){
        dst[0].userName[i] = src.userName[i];
    }
    for(int i = strlen(src.userName) ; i < 256 ; i++){
        dst[0].userName[i] = '\0';
    }
    for(int i = 0 ; i < strlen(src.password) ; i++){
        dst[0].password[i] = src.password[i];
    }
    for(int i = strlen(src.password) ; i < 256 ; i++){
        dst[0].password[i] = '\0';
    }
    dst[0].type = src.type;
    dst[0].userId = src.userId;
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

int updateUserData(USERS user, USERS **userList, int64_t listSize, int addUser){
    int64_t size = listSize;
    FILE *fp = fopen(dir, "wb");
    if(!fp)
        return -1;
    if(addUser == 1){
        size += 1;
    }
    if(fwrite(&size, sizeof(int64_t), 1, fp) != 1 ) return -1;
    if(fwrite(userList, sizeof(USERS), size, fp) != size) return -1;
    if(addUser == 1){
        if(fwrite(&user, sizeof(USERS), 1, fp) != 1 )return -1;
    }
    fclose(fp);
    return 1;
}

int64_t readTotalUsers(){
    int64_t totalUsers = 0;
    FILE *fp = fopen(dir, "rb");
    if(!fp){
        FILE *makeFile = fopen(dir, "wb");
        fclose(makeFile);
        fp = fopen(dir, "rb");
        if(!fp)
            return 0;
    }
    fseek(fp, 0, SEEK_END);
    if(ftell(fp) == 0){
        fclose(fp);
        return 1;
    }
    fseek(fp, 0, SEEK_SET);
    fread(&totalUsers, sizeof(int64_t), 1, fp);
    fclose(fp);
    if(totalUsers == 0) totalUsers = 1;
    return totalUsers;
}

int loadUserData(USERS **userList, int64_t userTotal) {
    if(userTotal < 1)
        return -1;
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
    fread(&filesize, sizeof(int64_t), 1, fp);
    fread(*userList, sizeof(USERS)*userTotal, 1, fp);
    //copyUser(userList[i], user);
    fclose(fp);
    return 0;
}

void freeUserData(USERS **userList, int64_t userListSize) {
    if(userList == NULL)
        return;
    free(*userList);
    //for(int i = 0 ; i < userListSize ; i++){
    //    if(userList[i] == NULL) continue;
    //    free(userList[i]);
    //}
}

int createUser(char *username, char *password, int type){
    int64_t index = 0;
    int64_t userTotal = 0;
    userTotal = readTotalUsers();
    USERS *users = (USERS *)malloc(sizeof(USERS) * userTotal);
    if(users == NULL){ 
        return -1;
    }
    USERS user;
    user = setUser();
    switch(loadUserData(&users, userTotal)){
    case -1:
        return 0;
    case 1:
        strcpy(user.userName, username);
        strcpy(user.password, password);
        user.type = 100;
        user.userId = 1;
        copyUser(users, user);
        updateUserData(user, &users, 1, 0);
        freeUserData(&users, userTotal); return 1;
    default:
        USERS *check = NULL;
        check = searchUsername(&users, userTotal, username, &index);
        if(check != NULL){
            return -1;
        }
        strcpy(user.userName, username);
        strcpy(user.password, password);
        user.type = type;
        user.userId = users[userTotal-1].userId + 1;
        updateUserData(user, &users, userTotal, 1);
        freeUserData(&users, userTotal);
        return 1;
        break;
    }
}

int updateUser(int id, char *username, char *password, int type){
    int64_t userTotal = 0;
    int64_t index = 0;
    USERS *check = NULL;
    USERS user;
    userTotal = readTotalUsers();
    USERS *users = (USERS *)malloc(sizeof(USERS) * userTotal);
    if(users == NULL){ 
        return -1;
    }
    switch(loadUserData(&users, userTotal)){
        case -1:
            return -1;
        case 1:
            return 1;
        default:
            check = searchId(&users, userTotal, id, &index);
            if(!check)return 2;
            if(username != NULL)
                strcpy(users[index].userName, username);
            if(password != NULL)
                strcpy(users[index].password, password);
            if(type > 0)
                users[index].type = type;
            updateUserData(user, &users, userTotal, 0);
            freeUserData(&users, userTotal);
            return 0;
        }
}

int deleteUser(int id){
    int64_t index = 0;
    USERS *check = NULL;
    USERS user;
    int64_t userTotal = 0;
    userTotal = readTotalUsers();
    USERS *users = (USERS *)malloc(sizeof(USERS) * userTotal);
    if(users == NULL){ 
        return -1;
    }
    switch(loadUserData(&users, userTotal)){
        case -1:
            return -1;
        case 1:
            return 1;
        default:
            check = searchId(&users, userTotal, id, &index);
            if(!check)return 0;
            // aray[x] replaced with [x+n];
            // because i dont know how to do it better kek;
            if(index != userTotal-1){
                for(int i = index ; i < userTotal-1 ; i++){
                    users[i] = users[i+1];
                }
            }
            updateUserData(user, &users, userTotal-1, 0);
            freeUserData(&users, userTotal);
            return 0;
    }
}

int userValidate(char *username,char *password, USERS *user){
    int64_t userTotal = 0;
    int checks = 0;
    userTotal = readTotalUsers();
    USERS *users = (USERS *)malloc(sizeof(USERS) * userTotal);
    if(users == NULL){ 
        return -1;
    }
    *user = setUser();
    switch(loadUserData(&users, userTotal)){
        case -1:
            return 1;
        case 1:
            return 1;
        default:
            for(int i = 0 ; i < userTotal ; i++){
                int j = 0;
                int uLen = strlen(users[i].userName);
                if(uLen != strlen(username)) continue;
                for(j = 0 ; j < uLen ; j++){
                    if(users[i].userName[j] != username[j]) break;
                }
                if(j != uLen) continue;
                int plen = strlen(users[i].password);
                if(plen != strlen(password)) continue;
                for(j = 0 ; j < plen ; j++){
                    if(users[i].password[j] != password[j]) break;
                }
                if(j != plen) continue;
                strcpy(user->userName, users[i].userName);
                strcpy(user->password, users[i].password);
                user->type = users[i].type;
                user->userId = users[i].userId;
                break;
            }
            freeUserData(&users, userTotal);
            return 0;
    }
}
