#include "../structs.h"

#ifndef LOGIN_H
#define LOGIN_H

USERS *searchUsername(USERS **userList, int64_t listSize , char *username, int64_t *index);

USERS *searchId(USERS **userList, int64_t listSize, int id, int64_t *index) ;

USERS setUser();

int createUser(char *username, char *password, int type);

int updateUser(int id, char *username, char *password, int type);

int deleteUser(int userId);

int userValidate(char *username,char *password, USERS *user);

#endif
