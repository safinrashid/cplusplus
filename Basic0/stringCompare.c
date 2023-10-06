#include <stdio.h>

void toLowerCase(char* my_string) {
    char* curr_char = my_string;
    while(*curr_char != '\0') {
        if(*curr_char >= 65 && *curr_char <= 90) { // Between 'A' and 'Z'
            *curr_char = (char)(*curr_char + 32);
        }
        curr_char++;
    }
}

int compareStrings(const char* string_a, const char* string_b) {
    int index = 0;
    while(*(string_a + index) != '\0') {
        if(*(string_a + index) != *(string_b + index)) {
            return 0;
        }
        index++;
    }
    return 1;
}


int main() {
    char string_a[] = "HeLLo WoRld!";
    char string_b[] = "hEllO woRlD!";

    toLowerCase(string_a);
    toLowerCase(string_b);

    if(compareStrings(string_a, string_b) == 0) {
        printf("These strings are not the same!");
    }
    else {
        printf("These strings are the same!");
    }

    return 0;
}
