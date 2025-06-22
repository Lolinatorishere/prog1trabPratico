#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/Middleware/stringParse.h"
#include "../headers/Middleware/screenPrint.h"
#include "../headers/Middleware/menu.h"
#include "../headers/Middleware/user.h"
#include "../headers/structs.h"

int main(){
    int programState = 0;
    int start = 0;
    USERS user;
    if(start == 0){
        user = setUser();
        start = 1;
    }
    while(1){
        switch(programState){
            case 0:
                startUI(&programState);
                continue;
            case 1:
                login(&user, &programState);
                continue;
            case 2:
                NewUser(&programState, 0);
                continue;
            case 3:
                LoggedIn(&user, &programState);
                continue;
            default: 
                return 0;
        }
    }
}
