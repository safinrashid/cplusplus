// Set <Project5.cpp>
// ECE 312 Project 5 submission by
// Safin Rashid
// srr3288
// Slip days used: <0>
// Fall 2022

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"


///-------DONE--------
void destroySet(Set* self) {
    free(self->elements);
}
void createEmptySet(Set* self) {
    self->len = 0;
    self->elements = nullptr;
}
void createSingletonSet(Set* self, int x) {
    self->elements = (int*) malloc(sizeof(int));
    self->elements[0] = x;
    self->len = 1;
}
void createCopySet(Set* self, const Set* other) {
    self->elements = (int*) malloc(other->len * sizeof(int));
    for (int k = 0; k < other->len; k += 1) {
        self->elements[k] = other->elements[k];
    }
    self->len = other->len;
}
void assignSet(Set* self, const Set* other) {
    if (self == other) { return; }

    destroySet(self);
    createCopySet(self, other);
}
bool isEmptySet(const Set* self) {
    return self->len == 0;
}
void displaySet(const Set* self) {
    int k;

    printf("{");

    if (self->len == 0) {
        printf("}");
    }
    else {
        for (k = 0; k < self->len; k += 1) {
            if (k < self->len - 1) {
                printf("%d,", self->elements[k]);
            } else {
                printf("%d}", self->elements[k]);
            }
        }
    }
}

/* return true if x is an element of self */
///-----O(log(N))----- (binary search since sorted)
bool isMemberSet(const Set* self, int x) {
    int len  = self->len;
    if(len==0 || (len==1 && self->elements[0] != x)){return 0;}
    if(len==1 && self->elements[0] == x){return 1;}
    int low = 0, high = len-1;
    while(low <= high){
        int mid = (low + high)/2;
        int index = self->elements[mid];
        if(index < x){
            low = mid+1;
        }
        else if(index > x){
            high = mid-1;
        }
        else{//index == x
            return 1;
        }
    }
    return 0;
}

/*
 * add x as a new member to this set.
 * If x is already a member, then self should not be changed
 * Be sure to restore the design invariant property that elements[] remains sorted
 * (yes, you can assume it is sorted when the function is called, that's what an invariant is all about)
 */
///-----O(N)-----
void insertSet(Set* self, int x) {
    if(isMemberSet(self, x)) {return;}//already a member
    int sLen = self->len;
    if(!sLen){
        createSingletonSet(self, x);
        return;}
    //find new spot for x
    int index = 0, end = sLen - 1;
    if (x > self->elements[end]) {
        index = sLen;
    }else if(!(x < self->elements[0])){
        for (index = 1; index < end; index++) {
            if (x > self->elements[index - 1] && x < self->elements[index]){break;}
        }
    }
    //new set with x insertion
    int newLength = sLen + 1;
    int *selfNew = (int *) malloc(sizeof(int) * newLength);
    for (int i = 0; i < newLength; i++){
        if(i < index){
            selfNew[i] = self->elements[i]; //insert larger than x
        }else if(i > index) {
            selfNew[i] = self->elements[i - 1]; //insert greater than x
        }else{//i == index
            selfNew[i] = x;
        }
    }
    //free and update
    destroySet(self);//destroy old self
    self->elements = selfNew;//add new self with x
    self->len = newLength;  //update new length
}

/*
 * don't forget: it is OK to try to remove an element
 * that is NOT in the set.
 * If 'x' is not in the set 'self', then
 * removeSet should do nothing (it's not an error)
 * Otherwise, ('x' IS in the set), remove x. Be sure to update self->length
 * It is not necessary (nor recommended) to call malloc -- if removing an element means the
 * array on the heap is "too big", that's almost certainly OK, and reallocating a smaller array
 * is almost definitely NOT worth the trouble
 */
///-----O(N)-----
void removeSet(Set* self, int x) {
    if(isMemberSet(self, x)){//only remove if in the set
        int i = 0, len = self->len;
        for(i; i < len; i++){//find index of x
            if(self->elements[i] == x){break;}
        }
        len--;
        for(int j = 0; j < len; j++){//shift everything after x to the left
            if(j >= i){//after x, shift
                self->elements[j] = self->elements[j + 1];
            }
        }
        self->len = len;
    }
}

/* return true if self and other have exactly the same elements */
///-----O(N)-----
bool isEqualToSet(const Set* self, const Set* other) {
    int sLen = self->len, oLen = other->len;
    if((sLen != oLen) || (sLen == 0 xor oLen == 0)){return 0;}//if diff size or any empty set
    for(int i = 0; i < sLen; ++i){
        if(self->elements[i] != other->elements[i]  ||  sLen != oLen){//check each element, will break if different size
            return 0;
        }
    }
    return 1;
}

/* return true if every element of self is also an element of other */
///-----O(N)-----
bool isSubsetOf(const Set* self, const Set* other) { ///O(n)
    int s = 0, o = 0, sLen = self->len, oLen = other->len;
    while(sLen > s) {
        if(o >= oLen){//if other is empty or finished searching match
            return 0;
        }else if(self->elements[s] > other->elements[o]){//possibly, check next
            o++;
        }else if(self->elements[s] < other->elements[o]){//if less than, impossible to match
            return 0;
        }else if(self->elements[s] == other->elements[o]){//match
            s++;o++;
        }
    }
    return 1;
}


///Algebraic test fails, uses intersect, sub, union:


/* remove all elements from self that are not also elements of other */
///-----O(N)----- (already sorted)
void intersectFromSet(Set* self, const Set* other) { //self = self ∩ other
    int sLen = self->len, oLen = other->len;
    int s = 0, o = 0, i = 0;
    int* intersect = (int*) malloc(sizeof(int) * oLen);  //output array max length is other length
    while(s < sLen && o < oLen){
        if(self->elements[s] == other->elements[o]){//if equal, insert to intersection
            intersect[i] = self->elements[s];
            s++;o++;i++;
        }
        else if(self->elements[s] < other->elements[o]){s++;} //if self outside of other, check next self
        else if(self->elements[s] > other->elements[o]){o++;} //vise versa
    }
    //destroy and update struct info
    destroySet(self);
    self->elements = intersect;
    self->len = i;
}

/* remove all elements from self that are also elements of other */
///-----O(N)----- (already sorted)
void subtractFromSet(Set* self, const Set* other) {//self = self - other = self - (self ∩ other)
    int sLen = self->len, oLen = other->len, s = 0, o = 0, i = 0;
    int* subway = (int*) malloc(sizeof(int) * sLen);  //output array max length is self length
    while(s < sLen){//order of conditional matters to prevent illegal read
        if(oLen <= o || self->elements[s] < other->elements[o]){//not possible for self to be in other, add
            subway[i] = self->elements[s];
            i++;s++;
        }else if(self->elements[s] > other->elements[o]){//possibly same at later index, check next
            o++;
        }else if(self->elements[s] == other->elements[o]){//same, don't add
            s++;o++;
        }
    }
    //destroy and update struct info
    destroySet(self);
    self->elements = subway;
    self->len = i;
}

/* add all elements of other to self (obviously, without creating duplicate elements) */
///-----O(N)-----
void unionInSet(Set* self, const Set* other) {//if in other but not in self, add to self
    int sLen = self->len, oLen = other->len;
    int maxLen = sLen + oLen; //max size if sets are unique
    int s = 0, o = 0, i = 0;
    int* onion = (int*) malloc(sizeof(int) * maxLen);  //output array, union is keyword
    //self =  0 1 2 3 4                 //sLen = 5
    //other =           5 6 7 8 9 10    //oLen = 6
    //onion = 0 1 2 3 4 5 6 7 8 9 10    //expected
    //printf("sLen = %d\toLen = %d\n", sLen, oLen);
    while(s < sLen || o < oLen){//until both sets exhausted
        //do before to prevent reading outside of set
        if(s >= sLen){//finished checking/empty self, add rest of other
            onion[i] = other->elements[o];
            o++;i++;
        }else if(o >= oLen){//finished checking/empty other, add rest of self
            onion[i] = self->elements[s];
            s++;i++;
        }
        else if(self->elements[s] > other->elements[o]){//if greater, out of self range, add other
            onion[i] = other->elements[o];
            o++;i++;
        }else if(self->elements[s] <= other->elements[o]) {//if less than/out of other range, add self; if equal, add either
            onion[i] = self->elements[s];
            if (self->elements[s] == other->elements[o]) { o++; }//if equal, increment both
            s++;i++;
        }
    }
    //destroy and update struct info
    destroySet(self);
    self->elements = onion;
    self->len = i;
}
