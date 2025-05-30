#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/structs.h"

int loadUserList(const char *dir, USERS **userList) {
    FILE *file = fopen(dir, "rb");
    if (!file)
        return -1;
    USERS *head = NULL;
    USERS *tail = NULL;
    USERS *tempUser = NULL;
    while (fread(&tempUser, sizeof(USERS), 1, file) == 1) {
        USERS *newUser = (USERS *)malloc(sizeof(USERS));
        if (!newUser) {
            fclose(file);
            return -1;
        }
        newUser->next = NULL;
        if (!head) {
            head = newUser;
        } else {
            tail->next = newUser;
        }
        tail = newUser;
    }
    fclose(file);
    *userList = head;
    return 0;
}

void freeUserList(USERS *userList) {
    USERS *current = userList;
    while (current) {
        USERS *next = current->next;
        free(current);
        current = next;
    }
}

USERS *searchUsername(USERS *userList,char *username) {
    USERS *current = userList;
    while (current) {
        if (strcmp(current->userName, username) == 0) {
            return current; 
        }
        current = current->next;
    }
    return NULL;
}

USERS *searchId(USERS *userList,int id) {
    USERS *current = userList;
    while (current) {
        if (current->userId == id) {
            return current;
        }
        current = current->next;
    }
    return NULL; 
}

int userValidate(char *username,char *password){
    USERS *userList = NULL;
    char *dir = "./data/userData.dat";
    if (loadUserList(dir, &userList) == 0) {
        USERS *current = userList;
        while (current) {
            if (strcmp(current->userName, username) == 0 && strcmp(current->password, password) == 0) {
                freeUserList(userList);
                return 1;
            }
            current = current->next;
        }
    } else {
        freeUserList(userList);
        printf("Failed to load user list from %s\n",dir);
        return 0;
    }
}
