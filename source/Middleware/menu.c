#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
//#include "../headers/stringParse.h"
#ifdef _WIN32
    #define syscls system("cls")
#endif
#ifdef __linux__
    #define syscls system("clear")
#endif
#define TXT_CONST 56
#define INDENT 4
#define MARGIN 4

//prints a line of "space" aka " "
void dynamic_linespace(int text_constant, int txt_indent, int txt_margin, int lines){
    if(lines == 0) return;
    for(int i = 0 ; i < lines ; i++){
        char space[1] = " ";
        printf("|%*c|\n", text_constant + txt_indent + txt_margin, space[0]);
    }
}

void dynamic_begin_text(int text_constant, int txt_indent, int txt_margin){
    char line[1] = "-";
    printf("/");
    for(int i = 0; i < text_constant + txt_indent + txt_margin; i++){
        printf("%c",line[0]);
    }
    printf("\\\n");
}

void dynamic_end_text(int text_constant, int txt_indent, int txt_margin){
    char line[1] = "-";
    printf("\\");
    for(int i = 0; i < text_constant + txt_indent + txt_margin; i++){
        printf("%c",line[0]);
    }
    printf("/\n");
}

void dynamic_line_print(char *string, int text_const, int txt_indent, int txt_margin){
    //have to check for \n chars and replace with pretty much a dymanic line space
    //recieves a string that is read and then it is printed in a certain way
    int arraysize = strlen(string);
    if(arraysize == 0) return;
    if(text_const == 0) text_const = 1;
    char space[1] = " ";
    char buffer[256] = {'\0'};
    int i = 0, j = 0;
    printf("|%*c", txt_indent, space[0]);
    for(i = 0 ; i <= arraysize; i++){
        if(string[i] == '\n' && i+1 != arraysize){
            printf("%*c|\n", text_const-j + txt_margin, space[0]);
            j = 0;
            printf("|%*c", txt_indent, space[0]);
            continue;
        }
        if(j >= text_const){
            printf("%*c|\n", text_const-j + txt_margin, space[0]);
            printf("|%*c", txt_indent, space[0]);
            i--;
            j = 0;
            continue;
        }
        if(i+1 == arraysize){
            printf("%*c|\n", text_const-j + txt_margin, space[0]);
            continue;
        }
        j++;
        printf("%c",string[i]);
    }
}

int readMenuFile(char *menuSection, char **menuText){
    char *dir = malloc(sizeof(char)*256);
    if(!dir)return -1;
    strcpy(dir, "./menus/");
    strcat(dir, menuSection);
    strcat(dir,".menu");
    FILE *fp = fopen(dir, "r");
    if(fp == NULL) return -1;
    free(dir);
    fseek(fp, 0, SEEK_END);
    int64_t filesize = ftell(fp);
    if(filesize == 0){
        fclose(fp);
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    *menuText = malloc(sizeof(char) * filesize);
    if(!menuText)return -1;
    for(int i = 0 ; i < filesize ; i++){
        fseek(fp, i, SEEK_SET);
        (*menuText)[i] = fgetc(fp);
    }
    fclose(fp);
    return 0;
}

void printToScreen(char *input, int padding_top, int padding_bottom){
    syscls;
    dynamic_begin_text(TXT_CONST, INDENT, MARGIN);
    dynamic_linespace(TXT_CONST, INDENT, MARGIN, padding_top);
    dynamic_line_print(input, TXT_CONST, INDENT, MARGIN);
    dynamic_linespace(TXT_CONST, INDENT, MARGIN, padding_bottom);
    dynamic_end_text(TXT_CONST, INDENT, MARGIN);
}

int menuPrint(char *menuSection, int padding_top, int padding_bottom){
    char *menuText = NULL;
    if(readMenuFile(menuSection, &menuText) != 0) return -1;
    printToScreen(menuText, padding_top, padding_bottom);
    free(menuText);
    return 0;
}

int advancedPrint(char *input, int padding_top, int padding_bottom){
    if(input == NULL) return -1;
    printToScreen(input, padding_top, padding_bottom);
    return 0;
}
