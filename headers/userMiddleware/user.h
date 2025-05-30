#include "../structs.h"

#ifndef LOGIN_H
#define LOGIN_H

int loadUserList(char *dir, USERS **userList);

void freeUserList(USERS *userList);

USERS *searchUsername(USERS *userList, char *username);

USERS *searchId(USERS *userList,int id);

int userValidate(char username,char password);

#endif
