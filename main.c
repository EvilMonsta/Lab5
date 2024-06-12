#include <stdio.h>
#include "tasks.h"
#include "functions.h"
int main() {
    int tasknumber;
    do{
        printf("\nEnter task number[1,2] or [0] to exit the program:");
        tasknumber = checkInt();
        tasknumber = TaskNumCheck( tasknumber);
        if(tasknumber == 1)
            taskOne();
        else if(tasknumber == 2)
            taskTwo();
        tasknumber = ContinueWish(tasknumber);
    }
    while(tasknumber!=0);
    return 0;
}