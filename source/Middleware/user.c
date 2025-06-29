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

USERS *searchUsername(USERS *userList, int64_t listSize, char *username, int64_t *index) {
    int ulen = strlen(username);
    for(int64_t i = 0 ; i < listSize ; i++){
        int check = 0;
        if(ulen != strlen(userList[i].userName))continue;
        for(int j = 0 ; j < ulen; j++){
            if(userList[i].userName[j] != username[j])break;
            check++;
        }
        if(check != ulen) continue;
        return &userList[i];
    }
    return NULL;
}

//doesnt work big sad 
//todo work on part search of usernames
int deepUsernameSearch(USERS *userList, USERS *hits, int64_t listSize, int64_t *hit_size, char *userName){
    int unamesize = strlen(userName);
    int accuracy = unamesize*0.75;
    for(int64_t i = 0 ; i < listSize ; i++){
        int listnamesize = strlen(userList[i].userName);
        int same = 0;
        for(int j = 0 ; j < listnamesize ; j++){
            if(same > accuracy) {j = listnamesize; continue;}
            if(j > unamesize)continue;
            if(userList[i].userName[j] == userName[j]) same++;
        }
        if(same > accuracy){
            hits[*hit_size] = userList[i];
            *hit_size++;
        }
    }
    return 0;
}

int searchWithType(USERS *userList, USERS *hits, int64_t listSize, int64_t *hit_size, int type){
    int64_t total = 0;
    for(int i = 0 ; i < listSize ; i++){
        if(userList[i].type != type)continue;
        hits[total] = userList[i];
        total++;
    }
    *hit_size = total;
    return 0;
}

//This is assuming the ids in the List are sequential0
USERS *searchId(USERS *userList, int64_t listSize, int id, int64_t *index) {
    int64_t low = 0, high = listSize - 1;
    while (low <= high) {
        int64_t mid = low + (high - low) / 2;
        if (userList[mid].userId == id) {
            *index = mid;
            return &userList[mid];
        } else if (userList[mid].userId < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
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
    // 700 total for memory reasons, dont want buffer overflows
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
    int ut_type = strlen(" | Tipo:");
    int ut_name = strlen(" | Nome:");
    int ut_pwd = strlen(" \\ Pwd:");
    strcpy((*string), "\0");
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
                free(users);
                return 0;
            }
            break;
        case -1:
            free(users);
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
    free(users);
    return 1;
}

int updateUser(int id, char *username, char *password, int *type){
    if(username[0] == '\0' && password[0] == '\0' && type == NULL)return 1;
    int error = 0;
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
                error = 2;
                break;
            }
            if(username[0] != '\0'){
                int64_t data = 0;
                if(searchUsername(users, userTotal, username, &data) != NULL) {
                    if(users != NULL) free(users);
                    error = 3;
                    break;
                }
                username[strlen(username)] = '\0';
                strcpy(users[index].userName, username);
            }
            if(password[0] != '\0'){
                password[strlen(password)] = '\0';
                strcpy(users[index].password, password);
            }
            if(type != NULL)
                users[index].type = *type;
            updateUserData(users, userTotal);
            break;
    }
    free(users);
    return error;
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
                free(users);
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
            free(users);
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
        free(users);
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
    free(users);
    if(user->type < 0) return -1;
    return 0;
}

int addPageInfo(char **string, int page, int usersPerPage, int userTotal, char *special){
    char *pageExtras = malloc(sizeof(char)*256);
    char pageInfo[256] = {'\0'};
    char pageCur[256] = {'\0'};
    char pagetotal[1024] = {'\0'};
    int maxUserPrint = 0;
    int maxPages = userTotal/usersPerPage;
    strcpy(pageExtras, "\n+ pagina seguinte \n- pagina anterior\n");
    if(special != NULL){
        char *tmpctrls = realloc(pageExtras, sizeof(char) * (256 + strlen(special)));
        if(!tmpctrls){
            free(pageExtras);
            return -1;
        }
        pageExtras = tmpctrls;
        strcat(pageExtras, special);
    }
    strcat(pageExtras, "0 sair\n");
    if(userTotal%usersPerPage != 0) maxPages++;
    if((page+1) * usersPerPage > userTotal){
        maxUserPrint = userTotal;
    }else{
        maxUserPrint = usersPerPage*(page+1);
    }
    sprintf(pageCur,"pagina %i de %i", (page+1), maxPages);
    sprintf(pageInfo,"users %i a %i", (page*usersPerPage)+1, maxUserPrint);
    if(strlen(pageCur) + strlen(pageInfo) < TXT_CONST){
        centerString(TXT_CONST/2, pageCur);
        centerString(TXT_CONST/2, pageInfo);
        strcat(pagetotal, pageCur);
        pagetotal[strlen(pageCur)] = '|';
        strcat(pagetotal, pageInfo);
    }else{
        pageCur[strlen(pageCur)+1] = '\n';
        pageCur[strlen(pageCur)+2] = '\0';
        strcat(pagetotal, pageCur);
        pageCur[strlen(pageInfo)+1] = '\n';
        pageCur[strlen(pageInfo)+2] = '\0';
        strcat(pagetotal, pageInfo);
    }
    strcat(pagetotal, "\n");
    strcat(pagetotal, "\n");
    //32 for safety reasons;
    char *move = malloc(sizeof(char)*strlen((*string)));
    if(!move)
        return -1;
    strcpy(move, (*string));
    char *tmppage = realloc((*string), sizeof(char) * (strlen((*string)) + strlen(pagetotal) + strlen(pageExtras) + 32));
    if(!tmppage){
        free(pageExtras);
        return -1;
    }
    (*string) = tmppage ;
    strcpy((*string), pagetotal);
    strcat((*string), move);
    strcat((*string), pageExtras);
    free(pageExtras);
    free(move);
    return 0;
}

int showAllUsers(char **string, int usersPerPage, int *page, char *extras){
    int64_t userTotal = readTotalUsers();
    if(userTotal == 0) return -1;
    int maxPages = userTotal/usersPerPage;
    if(userTotal%usersPerPage != 0){
        maxPages++;
    }
    if (*page >= maxPages) *page = maxPages-1;
    if(*page<0)*page = 0;
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    if(!users)return -1;
    if(loadUserData(users) != 0){
        free(users);
        return -1;
    }
    if(createUserString(string, users, userTotal, usersPerPage, *page) != 0){
        free(users);
        return -1;
    }
    if(addPageInfo(string, *page, usersPerPage, userTotal, extras) != 0){
        free(users);
        free((*string));
        return -1;
    }
    free(users);
    return 0;
}

// todo move user pointer to outside so i dont have to repeat the search every fkn time
int searchForUsername(char **string, char *search, int usersPerPage, int page){
    int64_t userTotal = readTotalUsers();
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    int64_t index = 0;
    if(userTotal == 0) return -1;
    if(loadUserData(users) != 0){
        free(users);
        return 1;
    }
    USERS *user = searchUsername(users, userTotal, search, &index);
    if(!user){
        //todo work with deepUsernameSearch instead
        free(users);
        return 1;
    }
    if(createUserString(string, user, 1, 1, 0) != 0){
        free(users);
        return -1;
    }
    free(users);
    return 0;
}

int searchForUserId(char **string, int search, int usersPerPage, int page){
    int64_t userTotal = readTotalUsers();
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    int64_t index = 0;
    if(userTotal == 0) return -1;
    if(loadUserData(users) != 0){
        free(users);
        return 1;
    }
    USERS *user = searchId(users, userTotal, search, &index);
    if(user == NULL){
        (*string) = malloc(sizeof(char) * 64);
        strcpy((*string), "Utilizador Nao Existe\n");
    }else{
        if(createUserString(string, user, 1, 1, 0) != 0){
            free(users);
            return -1;
        }
    }
    free(users);
    return 0;
}

int searchForUserType(char **string, USERS **userList, int *totalUsers, int search, int usersPerPage, int *page){
    if((*userList) != NULL){
        int maxPages = *totalUsers/usersPerPage;
        if(*totalUsers%usersPerPage != 0){
            maxPages++;
        }
        if(*page >= maxPages) *page = maxPages-1;
        if(*page<0)*page = 0;
        if(createUserString(string, (*userList), (*totalUsers), usersPerPage, *page) != 0){
            free((*string));
            free((*userList));
            return -1;
        }
        if(addPageInfo(string, *page, usersPerPage, *totalUsers, NULL) != 0){
            free((*string));
            free(userList);
            return -1;
        }
        return 0;
    }
    int64_t userTotal = readTotalUsers();
    int64_t hit_size = 0;
    if(userTotal == 0) return -1;
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    if(!users){
        free((*string));
        return -1;
    }
    *userList = malloc(sizeof(USERS) * (userTotal + 1));
    if(!userList){
        free((*string));
        free(users);
        return -1;
    }
    if(loadUserData(users) != 0){
        free((*string));
        free(users);
        return 1;
    }
    searchWithType(users, (*userList), userTotal, &hit_size, search);
    free(users);
    *totalUsers = hit_size;
    if(createUserString(string, (*userList), *totalUsers, usersPerPage, *page) != 0){
        free((*string));
        free(userList);
        return -1;
    }
    if(addPageInfo(string, *page, usersPerPage, *totalUsers, NULL) != 0){
        free((*string));
        free(userList);
        return -1;
    }
    return 0;
}

int getUserWithId(USERS *user, int id){
    int64_t userTotal = readTotalUsers();
    int64_t index = 0;
    USERS *temp;
    USERS *users = malloc(sizeof(USERS) * (userTotal + 1));
    if(loadUserData(users) != 0){
        free(users);
        return -1;
    }
    temp = searchId(users, userTotal, id, &index);
    if(temp == NULL){free(users); return 1;}
    copyUser(user, (*temp));
    return 0;
}
