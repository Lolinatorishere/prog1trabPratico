#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/structs.h"
#define dir "./data/userdata.dat"

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

USERS *searchId(USERS *userList, int id, int *jumps) {
    USERS *current = userList;
    while (current) {
        if (current->userId == id) {
            return current;
        }
        jumps++;
        current = current->next;
    }
    return NULL; 
}

int updateUserData(USERS *head) {
    FILE *file = fopen(dir, "wb");
    if (file == NULL) {
        printf("Failed to open file");
        return -1;
    }
    USERS *current = head;
    while (current != NULL) {
        // Write the current node's data to the file
        if (fwrite(current, sizeof(USERS), 1, file) != 1) {
            printf("Failed to write to file");
            fclose(file);
            return -1;
        }
        current = current->next;
    }
    fclose(file);
    return 0;
}

int loadUserData(USERS **userList) {
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

void freeUserData(USERS *userList) {
    USERS *current = userList;
    while (current) {
        USERS *next = current->next;
        free(current);
        current = next;
    }
}

void updateList(USERS **head, USERS *newNode) {
    if (*head == NULL) {
        *head = newNode;
        newNode->type = 10;
    } else {
        USERS *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

USERS *addUser(char *username, char *password) {
    USERS *newNode = (USERS *)malloc(sizeof(USERS));
    if (newNode == NULL) {
        printf("Failed to allocate memory for new user node");
        return NULL;
    }
    strncpy(newNode->userName, username, sizeof(newNode->userName) - 1);
    newNode->userName[sizeof(newNode->userName) - 1] = '\0';
    strncpy(newNode->password, password, sizeof(newNode->password) - 1);
    newNode->password[sizeof(newNode->password) - 1] = '\0';
    newNode->type = 0;
    newNode->next = NULL;
    return newNode;
}

int createUser(char *username, char *password){
    USERS *users = NULL;
    USERS *endNode = NULL;
    if(loadUserData(&users) != 0){
        return 0;
    };
    USERS *check = searchUsername(users, username);
    if(check != NULL){
        freeUserData(users);
        return -1;
    }
    USERS *user = addUser(username, password);
    while(users->next != NULL){
        endNode = users->next;
    }
    endNode->next = user;
    updateUserData(users);
    freeUserData(users);
    return 1;
}

int updateUser(int id, char *username, char *password, int type){
    int jumps = 0;
    USERS *check = NULL;
    USERS *users = NULL;
    if(loadUserData(&users) != 0){
        return -1;
    }
    USERS *user = searchId(users, id, &jumps);
    if(user == NULL){
        return -2;
    }
    if(username[0] != '\0'){
        check = searchUsername(users, username);
        if(check != NULL){
            return -3;
        }
        strcpy(user->userName, username);
    }
    if(password[0] != '\0'){
        strcpy(user->password, password);
    }
    if(type > 0){
        user->type = type;
    }
    return 1;
}

int deleteUser(int userId){
    int jumps = 0;
    USERS *users = NULL;
    if(loadUserData(&users) != 0){
        return 0;
    };
    USERS *user = searchId(users, userId, &jumps);
    if(user == NULL){
        return -1;
    }
    USERS *prev = NULL;
    if(users->next != NULL){
        prev = users->next;
    }else{
        return 0;
    }
    for(int i = 0 ; i < jumps ; i++){
        prev = prev->next;
    }
    prev->next = user->next;
    user->next = NULL;
    freeUserData(user);
    updateUserData(users);
    freeUserData(users);
    return 1;
}

int userValidate(char *username,char *password){
    USERS *userList = NULL;
    if (loadUserData(&userList) == 0) {
        USERS *current = userList;
        while (current) {
            if (strcmp(current->userName, username) == 0 && strcmp(current->password, password) == 0) {
                freeUserData(userList);
                return 1;
            }
            current = current->next;
        }
    } else {
        freeUserData(userList);
        printf("Failed to load user list from %s\n",dir);
        return 0;
     }
}
