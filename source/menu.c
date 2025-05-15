#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void dynamic_linespace(int text_constant, int txt_indent, int txt_margin){
    char space[1] = " ";
    printf("|%*c|\n", text_constant + txt_indent + txt_margin, space[0]);
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
    char textconst = text_const;
    if(text_const == 0) textconst = 1;
    char space[1] = " ";
    char buffer[256] = {'\0'};
    int i = 0, j = 0;
    printf("|%*c", txt_indent, space[0]);
    for(i = 0 ; i <= arraysize; i++){
        if(string[i] == '\n' && i+1 != arraysize){
            printf("%*c|\n", textconst-j + txt_margin, space[0]);
            j = 0;
            printf("|%*c", txt_indent, space[0]);
            continue;
        }
        if(j >= textconst){
            printf("%*c|\n", textconst-j + txt_margin, space[0]);
            printf("|%*c", txt_indent, space[0]);
            i--;
            j = 0;
            continue;
        }
        if(i+1 == arraysize){
            printf("%*c|\n", textconst-j + txt_margin, space[0]);
            continue;
        }
        j++;
        printf("%c",string[i]);
    }
}


int menuPrint(char *menuSection, int padding_top, int padding_bottom){
    syscls;
    char *dir = malloc(sizeof(char)*256);
    strcpy(dir, "./menus/");
    strcat(dir, menuSection);
    strcat(dir,".menu");
    FILE *fp = fopen(dir, "r");
    if(fp == NULL) return -1;
    fseek(fp, 0, SEEK_END);
    int64_t filesize = ftell(fp);
    if(filesize == 0) return -1;
    fseek(fp, 0, SEEK_SET);
    char *menuText = (char*)malloc(sizeof(char)*filesize);
    for(int i = 0 ; i < filesize ; i++){
        fseek(fp, i, SEEK_SET);
        menuText[i] = fgetc(fp);
    }
    dynamic_begin_text(TXT_CONST, INDENT, MARGIN);
    for(int i = 0 ; i < padding_top; i++){
        dynamic_linespace(TXT_CONST, INDENT, MARGIN);
    }
    dynamic_line_print(menuText, TXT_CONST, INDENT, MARGIN);
    for(int i = 0 ; i < padding_bottom; i++){
        dynamic_linespace(TXT_CONST, INDENT, MARGIN);
    }
    dynamic_end_text(TXT_CONST, INDENT, MARGIN);
    free(menuText);
    free(dir);
    fclose(fp);
}
