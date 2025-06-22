
void returnText(char *where, int HowLong){
    printf("voltar para %s em %i\n", where, HowLong);
    sleep(1);
    for(int i = 1 ; i < HowLong ; i++){
        printf("%i\n",HowLong - i);
        sleep(1);
    }
    return;
}
