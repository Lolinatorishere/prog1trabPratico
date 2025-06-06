#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

