#include <stdio.h>
#include <string.h>
int findSumPrimes(int x);

int main(void) {
    char userString[50];
    char* stringPointer = NULL;
    int j = 0;

    strcpy(userString, "take tall");

    stringPointer = strchr(userString, 't');
    while (stringPointer != NULL) {
        *stringPointer = 'f';
        stringPointer = strchr(userString, 't');
    }

    printf("%s\n", userString);

    return 0;
}