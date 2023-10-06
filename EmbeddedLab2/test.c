#include "Lab2.h"
#include <stdint.h>

int32_t dot(int32_t a[], int32_t b[], int32_t n){
  int32_t s=0;
  for(int32_t i=0;i<n;i++){
    s += a[i]*b[i];
  }
  return s;
}
int32_t aa[5],bb[5];
int main(void){
  int32_t result;
  result = dot(aa,bb,5);
  while(1){
  }
}