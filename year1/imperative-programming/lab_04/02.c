#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_NOTHING 0
#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);
void background_comments(int asciiChar, int* prevChar, int* ongoingLine, int* ongoingBlock, int* blockChars, int* separation);
void background_strings(int asciiChar, int *prevChar, int *ongoingDouble, int *ongoingSingle, int* strSep);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents() {
	int ongoingBlock = 0, ongoingLine = 0;
    int prevChar = 0, separation = 0, blockChars = 0, asciiChar = 0;
	int strSep = 0, ongoingSingle = 0, ongoingDouble = 0;
	int currentState = 0;

	while(!feof(stdin)) {
		int asciiChar = fgetc(stdin);

		background_comments(asciiChar, &prevChar, &ongoingLine, &ongoingBlock, &blockChars, &separation);
		if (!ongoingBlock && !ongoingLine) {
			background_strings(asciiChar, &prevChar, &ongoingDouble, &ongoingSingle, &strSep);
		}
		
		if (ongoingBlock) {
			currentState = IN_BLOCK_COMMENT;
		} else if (ongoingLine) {
			currentState = IN_LINE_COMMENT;
		} else if (ongoingDouble || ongoingSingle) {
			currentState = IN_STRING;
		} else {
			currentState = IN_NOTHING;
		}

	}
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

void background_comments(int asciiChar, int* prevChar, int* ongoingLine, int* ongoingBlock, int* blockChars, int* separation) {
	if (*ongoingBlock) {
		(*blockChars)++;
	}

	if (asciiChar != '/' && asciiChar != '*') {
		*separation = 1;
	}

	if (asciiChar == '/' && *prevChar == '/' && !(*ongoingBlock) && !(*ongoingLine)) {
		*ongoingLine = 1;
	} 
	
	if (*prevChar == '/' && asciiChar == '*'  && !(*ongoingBlock) && !(*ongoingLine)) {
		*ongoingBlock = 1;
		*blockChars = 2;
	} 
	
	if (*prevChar == '*' && asciiChar == '/' && *ongoingBlock && *separation && (*blockChars) > 3) {
		*ongoingBlock = 0;
	}
	
	if ((asciiChar == '\n' || asciiChar == '\r') && *ongoingLine && !(*ongoingBlock)) {
		*ongoingLine = 0;
	}  

	if (asciiChar == '\n' || asciiChar == '\r') {
		*prevChar = 0;
	} else {
		*prevChar = asciiChar;
	}
}

void background_strings(int asciiChar, int *prevChar, int *ongoingDouble, int *ongoingSingle, int* strSep) {
    if (asciiChar == '"' && !(*ongoingSingle) && !(*ongoingDouble)) {
        *ongoingDouble = 1;
        *strSep = 0;
    }

    if (asciiChar == '\'' && !(*ongoingSingle) && !(*ongoingDouble)) {
        *ongoingSingle = 1;
        *strSep = 0;
    }

    if ((*prevChar) != '\\' && asciiChar == '"' && *ongoingDouble && *strSep) {
        *ongoingDouble = 0;
    }

    if ((*prevChar) != '\\' && asciiChar == '\'' && *ongoingSingle && *strSep) {
        *ongoingSingle = 0;
    }

    if (asciiChar != '\'' && asciiChar != '"') {
        *strSep = 1;
    }
    
    (*prevChar) = asciiChar;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}