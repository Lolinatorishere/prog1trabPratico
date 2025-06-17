#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/defs.h"

int stringReplace(char *input, char *filter, char *replace){
    if(input == NULL) return 1;
    if(filter[0] == '\0') return 1;
    int i = 0;
    int replaced = 0;
    char *buffer = (char*)malloc(sizeof(char)*strlen(input));
    if(strlen(input) == 0){
        strcpy(input, "\0");
        return 0;
    }
    for(i = 0 ; i < strlen(input) ; i++){
        if(input[i] != filter[0] && replace[0] != '\0'){
            buffer[i - replaced] = input[i];
            continue;
        }else{
            buffer[i] = replace[0];
        }
        if(replace[0] == '\0') replaced++;
    }
    strcpy(input, buffer);
    free(buffer);
    return 0;
}
//int intNumberInput(char *input, int *output);

int64_t int64FromString(char *input){
    if(input == NULL) return -1;
    if(strlen(input) == 0)return -1;
    return atoll(input);
}

//int floatNumberInput(char *input, float *output);

double doubleFromString(char *input){
    if(input == NULL) return -1;
    if(strlen(input) == 0)return -1;
    return atof(input);
}

int centerString(int size, char *text){
    if(size < strlen(text)) return -1;
    if(!text) return -1;
    if(strlen(text) > TXT_CONST) return -1;
    char *buffer = malloc(sizeof(char)*TXT_CONST);
    int index = 0, i = 0;
    int textsize = strlen(text);
    int unbal = (size-textsize)%2;
    int margins = (size-textsize)/2;
    for(i = 0; i < margins; i++)
        buffer[i];
    index += i;
    strcat(buffer, text);
    index += textsize;
    for(i = 0; i < margins + unbal; i++)
        buffer[i+index];
    index += i;
    buffer[index+1] = '\0';
    strcpy(text, buffer);
    free(buffer);
    return 0;
}
