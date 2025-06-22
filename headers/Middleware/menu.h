#ifndef MENU_H
#define MENU_H
#include "../structs.h"

void login(USERS *user, int *programState);

void startUI(int *programState);

void NewUser(int *programState, int admin);

void LoggedIn(USERS *user, int *programState);

#endif
