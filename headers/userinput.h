#ifndef USERINPUT_H
#define USERINPUT_H
/*
 obtains user input and returns the string with the replace value
 if replace is empty or null will replace with nothing
*/
int textInputSanitize(char *input, char *filter, char *replace);

//int intNumberInput(char *input, int *output);

int64_t int64NumberInput(char *input);
//int floatNumberInput(char *input, float *output);

double doubleNumberInput(char *input);

#endif
