#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// void print_vector(int v[], int n) {
//     for (int i = 0; i < n; ++i) {
//         printf("%d ", v[i]);
//     }
//     printf("\n");
// }

#define MAX_LINE 128


int main(void) {
    int blockCnt = 0, lineCnt = 0;
    int ongoingBlock = 0, ongoingLine = 0;
    int prevChar = 0;
    int separation = 0;

    while(!feof(stdin)) {
        int asciiChar = fgetc(stdin);
        
        if (asciiChar == -1) {
            if (ongoingLine) {
                lineCnt++;
            }
            break;        
        } 

        if (asciiChar) {
            printf("%c %c %d %d\n", prevChar, asciiChar, ongoingBlock, ongoingLine);
        }

        if (asciiChar != '/' && asciiChar != '*') {
            separation = 1;
        }

        if (asciiChar == '/' && prevChar == '/' && !ongoingBlock && !ongoingLine) {
            ongoingLine = 1;
        } 
        
        if (prevChar == '/' && asciiChar == '*'  && !ongoingBlock && !ongoingLine) {
            ongoingBlock = 1;
        } 
        
        if (prevChar == '*' && asciiChar == '/' && ongoingBlock && separation) {
            blockCnt++;
            ongoingBlock = 0;
        }
        
        if ((asciiChar == '\n' || asciiChar == '\r') && ongoingLine && !ongoingBlock) {
            lineCnt++;
            ongoingLine = 0;
        }  

        if (asciiChar == '\n' || asciiChar == '\r') {
            prevChar = 0;
        } else {
            prevChar = asciiChar;
        }
    }
    printf("\nBlock comments: %d\nLine comments: %d\n", blockCnt, lineCnt);
}
