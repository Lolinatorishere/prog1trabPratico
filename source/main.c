#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/userinput.h"

int main(){
    char *test = (char*)malloc(sizeof(char)*25);
    strcpy(test,"this is a test 111111111");
    textInputSanitize(test, "1", '\0');
    printf("Hello World\n");
    printf("%li\n", int64NumberInput("7448954290"));
    printf("%.5f\n",doubleNumberInput("2385342.838749"));
    printf("%s", test);
    free(test);
}
