#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/structs.h"
#include "../../headers/Middleware/stringParse.h"
#define dir "./data/userData.dat"
#include "../../headers/defs.h"

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

USERS *searchUsername(USERS *userList, int64_t listSize , char *username, int64_t *index) {
    for(int64_t i = 0 ; i < listSize ; i++){
        if(strcmp(userList[i].userName, username) == 0){
            *index = i;
            return &userList[i];
        }
    }
    return NULL;
}

USERS *searchId(USERS *userList, int64_t listSize, int id, int64_t *index) {
    for(int64_t i = 0 ; i < listSize ; i++){
        if(userList[i].userId == id){
            *index = i;
            return &userList[i];
        }
    }
    return NULL;
}

int updateUserData(USERS userList[], int64_t listSize){
    FILE *fp = fopen(dir, "wb");
    if(!fp)
        return -1;
    if(fwrite(&listSize, sizeof(int64_t), 1, fp) != 1 ) return -1;
    for(int i = 0 ; i < listSize ; i++){
        if(fwrite(&userList[i], sizeof(USERS), 1, fp) != 1) return -1;
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
        return 0;
    }
    fseek(fp, 0, SEEK_SET);
    fread(&totalUsers, sizeof(int64_t), 1, fp);
    fclose(fp);
    if(totalUsers == 0) totalUsers = 1;
    return totalUsers;
}

int loadUserData(USERS *userList) {
    FILE *fp = fopen(dir, "rb");
    if(!fp) return -1;
    fseek(fp, 0, SEEK_END);
    if(ftell(fp) == 0){
        fclose(fp);
        return 1;
    }
    fseek(fp, 0, SEEK_SET);
    int64_t userTotal = 0;
    fread(&userTotal, sizeof(int64_t), 1, fp);
    if(userTotal == 0){
        fclose(fp);
        return 1;
    }
    for(int i = 0 ; i < userTotal ; i++){
        fread(&userList[i], sizeof(USERS), 1, fp);
    }
    fclose(fp);
    return 0;
}

void freeUserData(USERS **userList) {
    if(!userList)
        return;
    free(*userList);
}

//this is seperate because it can be reused for other funcitons if given other user arrays

int createUserString(char **string, USERS *users,int userTotal, int usersPerPage, int page){
    //64 for special chars like |,/,\,_,\n name, type, pwd and id
    // 256 * 2 name and pwd, 64, 32 for type and 32 for id all in chars
    // 700 total for securitiy reasons, dont want buffer overflows
    *string = malloc(sizeof(char) * (usersPerPage * (700)));
    /*
     * / Id:id
     * | Type: type
     * | name:
     * \ pwd:
    */
    int index = 0;
    char buffer[TXT_CONST];
    int ut_id = strlen(" / Id:");
    int ut_type = strlen(" | Type:");
    int ut_name = strlen(" | Name:");
    int ut_pwd = strlen(" \\ Pwd:");
    int wrap_sexy = 0;
    if(TXT_CONST-ut_name <= ut_name)return -1;
    for(int i = (page * usersPerPage); i < usersPerPage + (page * usersPerPage); i++){
        if(i >= userTotal) break;
        USERS user = users[i];
        strcat((*string), " / Id:"); index += ut_id;
        sprintf(buffer, "%i\n", user.userId);
        strcat((*string), buffer); index += strlen(buffer);
        strcat((*string), " | Type:"); index += ut_type;
        sprintf(buffer, "%i\n", user.type);
        strcat((*string), buffer); index += strlen(buffer);
        strcat((*string), " | Name:"); index += ut_name;
        int name_len = strlen(user.userName);
        if(name_len < TXT_CONST-ut_name){
            strcat((*string), user.userName); index += name_len;
        }else{
            for(int j = 0; j < name_len ; j++){//needs testing
                (*string)[j+index] = user.userName[j];
                if(j%TXT_CONST-ut_name == 0){
                    strcat((*string), " |      "); index += ut_name;
                    continue;
                }
                index++;
            }
        }
        strcat((*string), "\n"); index++;
        int pwd_len = strlen(user.password);
        if(pwd_len < TXT_CONST-ut_pwd){
            strcat((*string), " \\ Pwd:"); index += ut_pwd;
            strcat((*string), user.password); index += pwd_len;
        }else{
            strcat((*string), " | Pwd:"); index += ut_pwd;
            for(int j = 0; j < pwd_len; j++){
                (*string)[j+index] = user.password[j];
                if(j%TXT_CONST-ut_name == 0){
                    if(pwd_len - j < TXT_CONST - ut_pwd){
                        strcat((*string), " \\     "); index += ut_pwd;
                    }else{
                        strcat((*string), " |     "); index += ut_pwd;
                    }
                    continue;
                }
                index++;
            }
        }
        strcat((*string), "\n"); index++;
    }
    (*string)[index]='\0';
    (*string)[index+1]='\0';
    return 0;
}

int createUser(char *username, char *password, int type){
    int64_t userTotal = readTotalUsers();
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    int64_t index = 0;
    switch(loadUserData(users)){
        case 0:
            if(searchUsername(users, userTotal, username, &index) != NULL){
                freeUserData(&users);
                return 0;
            }
            break;
        case -1:
            freeUserData(&users);
            return -1;
        default:
            break;
    }
    USERS *newUser = &users[userTotal];
    strncpy(newUser->userName, username, 255);
    newUser->userName[255] = '\0';
    strncpy(newUser->password, password, 255);
    newUser->password[255] = '\0';
    if (userTotal == 0) newUser->type = 100;
    else newUser->type = type;
    if (userTotal == 0) newUser->userId = 1;
    else newUser->userId = users[userTotal - 1].userId + 1;
    userTotal++;
    if(updateUserData(users, userTotal) == -1)
        return -1;
    freeUserData(&users);
    return 1;
}

int updateUser(int id, char *username, char *password, int type){
    int64_t index = 0;
    int64_t userTotal = readTotalUsers();
    USERS *users = malloc(sizeof(USERS) * (userTotal+1));
    if(!users){ 
        return -1;
    }
    switch(loadUserData(users)){
        case -1:
            return -1;
        case 1:
            return 1;
        default:
            if(!searchId(users, userTotal, id, &index)){
                freeUserData(&users);
                return 2;
            } 
            if(username != NULL){
                int64_t data = 0;
                if(searchUsername(users, userTotal, username, &data) != NULL) {
                    freeUserData(&users);
                    return 2;
                }
                strcpy(users[index].userName, username);
            }
            if(password != NULL)
                strcpy(users[index].password, password);
            if(type > 0)
                users[index].type = type;
            updateUserData(users, userTotal);
            freeUserData(&users);
            return 0;
    }
}

int deleteUser(int id){
    int64_t index = 0;
    USERS *check = NULL;
    USERS user;
    int64_t userTotal = readTotalUsers();
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    if(!users) return -1;
    switch(loadUserData(users)){
        case -1:
            return -1;
        case 1:
            return 1;
        default:
            if(!searchId(users, userTotal, id, &index)){
                freeUserData(&users);
                return 0;
            }
            // aray[x] replaced with [x+n];
            // because i dont know how to do it better kek;
            if(index != userTotal-1){
                for(int i = index ; i < userTotal-1 ; i++){
                    users[i] = users[i+1];
                }
            }
            updateUserData(users, userTotal-1);
            freeUserData(&users);
            return 0;
    }
}

int userValidate(char *username,char *password, USERS *user){
    int64_t userTotal = 0;
    int checks = 0;
    userTotal = readTotalUsers();
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    if(!users) return -1;
    *user = setUser();
    if(loadUserData(users) != 0){
        freeUserData(&users);
        return 1;
    }
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
    freeUserData(&users);
    if(user->type < 0) return -1;
    return 0;
}

int showAllUsers(char **string, int usersPerPage, int page){
    int64_t userTotal = readTotalUsers();
    if(userTotal == 0) return -1;
    if(page*usersPerPage > userTotal)return -1;
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    if(!users)return -1;
    if(loadUserData(users) != 0){
        freeUserData(&users);
        return -1;
    }
    if(createUserString(string, users, userTotal, usersPerPage, page) != 0){
        freeUserData(&users);
        return -1;
    }
}

int getUserWithId(USERS *user, int id){
    if(user == NULL)return -1;
    int64_t userTotal = readTotalUsers();
    int64_t index = 0;
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    user = searchId(users, userTotal, id, &index);
    free(users);
    if(user == NULL)return -1;
    return 0;
}
