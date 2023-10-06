#include <stdio.h> // declarations of standard input/output functions
#include <stdint.h> // declarations of standard integer types (e.g., int32_t)
#include <stdlib.h> // declarations of general functions in the standard library (e.g., abort, exit)

#define ONE_MB (1 << 20)
#define BUFFER_SIZE ONE_MB

char superstrings[BUFFER_SIZE]; // a large buffer to hold a dictionary (1MB)
char strings[BUFFER_SIZE]; // a large buffer to hold an strings (1MB)

void printSuperStrings(char strings [], char superstrings []);  // this is the function you write

/*
 * Read a .txt file into one of the two buffers
 * this function does NOT confirm that the file is correctly formatted
 */
void readFile(const char file_name[], char buffer[]) {
    FILE* file = fopen(file_name, "r");
    if (file == 0) {
        printf("unable to open file \"%s\"\n", file_name);
        abort();
    }

    /* clear (set to zeros) the buffer */
    for (uint32_t k = 0; k < BUFFER_SIZE; k += 1) {
        buffer[k] = 0;
    }

    /* read the file (this is a binary read, which is OK) */
    uint32_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
    buffer[bytes_read] = 0;
}


void simpleTest(void) {
    printf("******* Starting Base Test #1 (simpleTest) *******\n");
    char strings1[] = "hello\t bye";
    char superstrings1[] = " h Hello\nbye\thellobye\nehlol xhxxexxllox "; // should print: Hello hellobye hxxexxllo bye hellobye (diff lines)
    printSuperStrings(strings1, superstrings1);
    printf("****DONE****\n\n");
    char strings2[] = "bulldog bulld BcdWord";
    char superstrings2[] = "bulldogx\nbulldog\nbulldo\nulldog\nxbullxdogxx\nbulsdf\nabuxllORdoxgM\nulld\nBcdword\nbcdWord\nBcddffgsdkwWord\nMcvnskdfasdAdds\ndscaksdIWBs\nAnwndchasda\nSndscsdfd\nLSdajs\nAjnsad";
    //should print: bulldogx bulldog xbullxdogxx abuxllORdoxgM bulldogx bulldog bulldo xbullxdogxx buxllORdoxgM (diff lines)
    printSuperStrings(strings2, superstrings2);
    printf("****DONE****\n\n");
    char strings3[] = " \t\n";
    char superstrings3[] = "\n \t";
    printSuperStrings(strings3, superstrings3);
    printf("Shouldn't print anything\n****DONE****\n");
}

void generalTest(void) {
    printf("\n\n******* Starting General Test #2 *******\n");
    readFile("strings1.txt", strings);
    readFile("superstrings1.txt", superstrings);
    printSuperStrings(strings, superstrings);
    printf("****DONE****\n\n");
}

/* you should also make your own tests! */
int main(void) {
    /* for starters, uncomment the following tests, one at a time */

    //simpleTest();
        generalTest();
}
