#include <stdio.h>
void printStrings(char* s[]) {
    int i = 0;
    while (s[i] != NULL) {
        printf("%s", s[i]);
        i += 1;
    }
}
