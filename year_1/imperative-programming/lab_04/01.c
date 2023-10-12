#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))
#define MAX_LINE 128

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

typedef struct {
    char c;
    int count;
} charCount;

int count[MAX_BIGRAMS] = { 0 };

int read_int();
void print_cc_vector(charCount v[], int n);
void swap_cc(charCount *a, charCount *b);
void swap(int *a, int *b);
void bubblesort_cc(charCount v[], int n);
void bubblesort(int v[], int n);

void print_vector(int v[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void print_char_vector(char v[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%c ", (char)v[i]);
    }
    printf("\n");
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    *nl = *nw = *nc = 0;
    
    int prevChar = 0;
    while(!feof(stdin)) {
        int asciiChar = fgetc(stdin);
        nc++;
        if (isspace(asciiChar) || asciiChar == -1) {
            if (!isspace(prevChar)) {
                nw++;
            }
        }
        if (asciiChar == 0x0a || asciiChar == NEWLINE) {
            nl++;
        };
        if (asciiChar == -1) {
            nl++;
            break;
        }
        prevChar = asciiChar;
    }
}

void char_count(int char_no, int *n_char, int *cnt) {
    char buffer[MAX_LINE];
    charCount counter[LAST_CHAR];
    
    // Initialize the counter array
    for (int i = 0; i < LAST_CHAR; i++) {
        counter[i].c = (char)(i);
        counter[i].count = 0;
    }

    // read the input and count the chars
    while (fgets(buffer, MAX_LINE, stdin) != NULL) {
        for (int i = 0; i < MAX_LINE; i++) {
            if (buffer[i] == '\0' || buffer[i] == NEWLINE || buffer[i] == '\r') {
                break;
            } else if ((int)buffer[i] >= FIRST_CHAR && (int)buffer[i] < LAST_CHAR) {
                counter[(int)buffer[i]].count++;
            }
        }
    }

    // sort the counter array and get the char_no-th char
    bubblesort_cc(counter, LAST_CHAR);
    *n_char = counter[char_no-1].c;
    *cnt = (int)counter[char_no-1].count;
}

void bigram_count(int bigram_no, int bigram[]) {
    int counter[MAX_BIGRAMS];
    int temp[MAX_BIGRAMS];
    char buffer[MAX_LINE];


    for (int i = 0; i < MAX_BIGRAMS; i++) {
        counter[i] = 0;
    }

    while (fgets(buffer, MAX_LINE, stdin) != NULL) {
        for (int i = 1; i < MAX_LINE; i++) {
            if (buffer[i] == '\0' || buffer[i] == NEWLINE || buffer[i] == '\r' || buffer[i-1] == '\0' || buffer[i-1] == NEWLINE || buffer[i-1] == '\r') {
                break;
            } else if ((int)buffer[i] >= FIRST_CHAR && (int)buffer[i] < LAST_CHAR) {
                if ((int)buffer[i-1] >= FIRST_CHAR && (int)buffer[i-1] < LAST_CHAR) {
                    counter[((int)buffer[i-1] - 33) * MAX_CHARS + (int)buffer[i] - 33]++;
                }
            }
        }
    }

    for (int i = 0; i < MAX_BIGRAMS; i++) {
        temp[i] = counter[i];
    }

    bubblesort(temp, MAX_BIGRAMS);

    int to_search = temp[bigram_no-1];
    for (int i = 0; i < MAX_BIGRAMS; i++) {
        if (counter[i] == to_search) {
            bigram[0] = i / MAX_CHARS + 33;
            bigram[1] = i % MAX_CHARS + 33;
            bigram[2] = counter[i];
            break;
        }
    }
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    int blockCnt = 0, lineCnt = 0;
    int ongoingBlock = 0, ongoingLine = 0;
    int prevChar = 0, separation = 0, blockChars = 0;

    while(!feof(stdin)) {
        int asciiChar = fgetc(stdin);
        
        if (asciiChar == -1) {
            if (ongoingLine) {
                lineCnt++;
            }
            break;        
        }

        if (ongoingBlock) {
            blockChars++;
        }

        if (asciiChar != '/' && asciiChar != '*') {
            separation = 1;
        }

        if (asciiChar == '/' && prevChar == '/' && !ongoingBlock && !ongoingLine) {
            ongoingLine = 1;
        } 
        
        if (prevChar == '/' && asciiChar == '*'  && !ongoingBlock && !ongoingLine) {
            ongoingBlock = 1;
            blockChars = 2;
        } 
        
        if (prevChar == '*' && asciiChar == '/' && ongoingBlock && separation && blockChars > 3) {
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
    *line_comment_counter = lineCnt;
    *block_comment_counter = blockCnt;
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc (&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

void print_cc_vector(charCount v[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("char %c cnt %d ", v[i].c, v[i].count);
    }
    printf("\n");
}

void swap_cc(charCount *a, charCount *b) {
    charCount temp = *a;
    *a = *b;
    *b = temp;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubblesort(int v[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (v[j] < v[j + 1]) {
                swap(&v[j], &v[j + 1]);
            }
        }
    }
}

void bubblesort_cc(charCount v[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (v[j].count < v[j + 1].count) {
                swap_cc(&v[j], &v[j + 1]);
            }
        }
    }
}

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}
