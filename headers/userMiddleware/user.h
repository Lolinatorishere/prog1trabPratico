#include "../structs.h"

#ifndef LOGIN_H
#define LOGIN_H

int loadUserData(USERS **userList);

int updateUserData(USERS **userList);

void freeUserData(USERS *userList);

int deleteUser(int id);

USERS *searchUsername(USERS *userList, char *username);

USERS *searchId(USERS *userList,int id, int *jumps);

int userValidate(char username,char password);

#endif
