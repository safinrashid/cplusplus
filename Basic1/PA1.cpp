/**
 * NAME: Safin Rashid
 * EID: srr3288
 * Fall 2022
 * Santacruz
*/

#include <stdio.h> // declarations of standard input/output functions
#include <stdint.h> // declarations of standard integer types (e.g., int32_t)
#include <stdlib.h> // declarations of general functions in the standard library (e.g., abort, exit)

//returns true if char is a letter
bool lettertf(char checkifletter){
    if(('A' <= checkifletter && checkifletter <= 'Z') || ('a' <= checkifletter && checkifletter <= 'z')){
        return true; //is a letter, any ase
    }else{return false;}} //not a letter
//returns true if same letter (NOT case-sensitive)
bool samelettertf(char char1, char char2){
    if(not lettertf(char1) || not lettertf(char2)){
        return false;//not letters
    }else if(char1 == char2){
        return true; //same letter, same case
    }else if((char1-32 == char2) || (char2-32 == char1)){
        return true; //same letter, diff case
    }else{return false;}}//corner
//returns num of words in string
int wordsin(char inputstr []){
    int returni = 0;
    for(int i = 0; inputstr[i] != '\0'; i++){
        if(lettertf(inputstr[i]) && not lettertf(inputstr[i+1])){
            returni++;}}
    return returni;}
//returns num of letters in a word given a pointer in a string; 0/false if whitespace
int lettersin(char &inputwrd){
    int lettercount = 0;
    char* letter = &inputwrd;
    for(int step = 0; lettertf(*(letter + step)); step++){
        lettercount++;}
    return lettercount;}
//prints a word using pointer to the first letter
void printword(char *inputwrd){
    for(int i = 0; i < lettersin(*inputwrd); i++){
        printf("%c", *(inputwrd + i));
    }
    printf("\n");}

//******************************************************************************************************************//

void printSuperStrings(char strings [], char superstrings []){
    int stringWords = wordsin(strings), superWords = wordsin(superstrings);
    if(stringWords == 0 || superWords == 0){return;}//EDGE case: no words
    int wordcount = 0; //debug

    for(int i = 0; strings[i] != 0; i++){//iterating thru strings, in a strings word

        int letinwrd = 0;
        wordcount++; if(wordcount > stringWords+1){break;}//EDGE case: prevent indexing out of string
        if(lettertf(strings[i])){//if a letter, at start of word
            letinwrd = lettersin(strings[i]);//count letters in word
            printf("%d ", letinwrd);printword(&strings[i]);//test (CMT OUT)

            for (int j = 0; superstrings[j] != 0; j++){//iterate through superstrings words

                int letinwrdss = 0;
                if(lettertf(superstrings[j])) {//if a letter
                    letinwrdss = lettersin(superstrings[j]); //letters in superstrings word

                    if(letinwrdss >= letinwrd){//can only be a super string with at least same num of letters
                        printf("  %d ", letinwrdss);printf("   ");printword(&superstrings[j]);//test (CMT OUT)

                        int checkcount = 0, l = 0;
                        for(int k = 0; k < letinwrd; k++){//iterate thru string word letters
                            char char2find = strings[i+k]; //string word letter

                            for(l; l < letinwrdss; l++){//iterate thru superstring word letters
                                char charinspect = superstrings[j+l]; //superstring word letter
                                if(samelettertf(char2find, charinspect)){//same letter found
                                    checkcount++;
                                    l++;
                                    break;//letter found -> go to next string letter to look for
                                }
                            }
                            if(checkcount == letinwrd){
                                printf("     -MATCH!: ");//TEST  (CMNT OUT)
                                printword(&superstrings[j]);//print superstring using printword
                            }
                        }
                    }
                }
                j += letinwrdss;//leave/next word in superstring
            }
        }
        i += letinwrd;//leave/next word in string
    }
}
