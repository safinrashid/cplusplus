// Recursion.h -- function declarations for the EE312 Recursion project

#ifndef _Recursion_h
#define _Recursion_h 1

int maxRec1(int[], int);
int maxRec2(int[], int);
double sqrtRec(double, double, double);
int strCompare(char*, char*);
int strCompare2(char*, char*);
int solveMazeRec(int, int);


/* this struct is also defined in the project file, please do not change the struct in either file, or you'll break the main program
 * (i.e., the two struct definitions must match each other EXACTLY)
 */
struct Martian {
    int pennies;
    int nicks;
    int dodeks;
};

Martian change(int);

Martian change(int, int, int);

#endif /* _Recursion_h */
