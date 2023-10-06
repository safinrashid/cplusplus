/**
 * NAME: Safin Rashid
 * EID: srr3288
 * Fall 2022
 * Santacruz
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "UTString.h"

/*
 * Helper macro to find the signature of a UTString
 * Treat this like a function that takes a UTString*
 * Accesses a uint32_t from where the signature should be located
 */

#define CHECK(s) (*(uint32_t*) ((s)->string + (s)->length + 1))

/*
 * Checks if a UTString is valid or not.
 */

bool isOurs(const UTString* s) {
    if (CHECK(s) == SIGNATURE) { return true; }
    else { return false; }
}

/*
 * Allocate a utstring on the heap.
 * Initialize the string correctly by copying src.
 * Return a pointer to the UTString.
 * Create a UTString on the heap that holds a copy of source,
 * setting the length, capacity, and check appropriately.
 * Return a pointer to the UTString.
 */
UTString* utstrdup(const char* src) {

    UTString* utstrheap = (UTString*) malloc(sizeof(UTString)); //allocate struct on heap

    int len_cap = strlen(src);  //set length and capacity, same for now
    utstrheap->length = len_cap;
    utstrheap->capacity = len_cap;

    utstrheap->string = (char*) malloc((len_cap + 1 + 4) * sizeof(char));    //allocate for string (1=null, 4=check)
    for(int i = 0; src[i] != 0; i++){
        utstrheap->string[i] = src[i]; //copy src string into struct string
    }
    utstrheap->string[len_cap] = 0;     //add null terminator
    CHECK(utstrheap) = SIGNATURE;       //add signature

    return utstrheap;
}

/*
 * Reverses the string in s.
 * Null and check should remain in the same location.
 * Only reverse everything before the \0.
 */
UTString* utstrrev(UTString* s) {
    assert(isOurs(s));

    int length = strlen(s->string);

    char strtemp[length + 1]; //size of string to include terminator
    for(int i = 0; i < length; i++){
        strtemp[i] = s->string[length-i-1]; //reverse string
    }
    strtemp[length] = 0; //add null terminator
    free(s->string);
    free(s);        //get rid of unreversed struct
    UTString* s_new = utstrdup(strtemp); //new struct with reversed str

    return s_new;
}

/*
 * Append the string suffix to the UTString s.
 *  s must be a valid UTString.
 * Do not allocate any additional storage: only append as many characters
 *  as will actually fit in s.
 * Update the length of s.
 * Return s with the above changes. */
UTString* utstrcat(UTString* s, const char* suffix) {
    assert(isOurs(s));

    for(int i = 0; (suffix[i]!=0) && (s->length < s->capacity); i++){ //before string null AND within capacity
        s->string[s->length] = suffix[i]; //append
        s->length++; //add to length until capacity reached
    }

    s->string[s->length] = 0; //move/add terminator
    CHECK(s) = SIGNATURE;

    return s;
}

/*
 * Copy src into dst.
 *  dst must be a valid UTString.
 *  src is an ordinary string.
 * Do not allocate any additional storage: only copy as many characters
 *  as will actually fit in dst.
 * Update the length of dst.
 * Return dst with the above changes.
 */
UTString* utstrcpy(UTString* dst, const char* src) {
    assert(isOurs(dst));

    int newlength = 0;

    for(int i = 0; (src[i] != 0) && (dst->capacity > i); i++){ //within src and within capacity
        newlength++;
        dst->string[i] = src[i]; //copy into dst
    }

    dst->length = newlength; //update length
    dst->string[newlength] = 0; //add null terminator
    CHECK(dst) = SIGNATURE;

    return dst;
}

/*
 * Free all memory associated with the given UTString.
 */
void utstrfree(UTString* self) {
    assert(isOurs(self));
    //order matters
    free(self->string);
    free(self);
}

/*
 * Make s have at least as much capacity as new_capacity.
 *  s must be a valid UTString.
 * If s has enough capacity already, do nothing.
 * If s does not have enough capacity, then allocate enough capacity,
 *  copy the old string and the null terminator, free the old string,
 *  and update all relevant metadata.
 * Return s with the above changes.
 */
UTString* utstrrealloc(UTString* s, uint32_t new_capacity) {
    assert(isOurs(s));

    if(s->capacity >= new_capacity){return s;} //do nothing
    else{

        int newrealloc = (new_capacity + 1 + 4) * sizeof(char);
        s->string = (char*) realloc(s->string, newrealloc); //realloc with new capacity
        s->capacity = new_capacity; //update capacity
        return s;

    }
}