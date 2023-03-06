#include <stdio.h>

char shiftChar(char x);

int main(void) {

    char string[20];
    scanf("%s", string);
    printf("The given string is: %s\n", string);
    for (int i = 0; i<20; i++) {
        if ((int)(string[i]) == 0) {
            break;
        }
        printf("%d\n", string[i]);
    }
    
}