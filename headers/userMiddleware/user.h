#include "../structs.h"

#ifndef LOGIN_H
#define LOGIN_H

USERS *searchUsername(USERS *userList,char *username);

USERS *searchId(USERS *userList,int id, int *jumps);

int createUser(char *username, char *password);

int updateUser(int id, char *username,char *password,int type);

int deleteUser(int userId);

int userValidate(char *username,char *password);

#endif
