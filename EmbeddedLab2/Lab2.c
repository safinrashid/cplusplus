// ****************** Lab2.c ***************
// Program written by: put your names here
// Date Created: 1/18/2017 
// Last Modified: 2/1/2022
#include "Lab2.h"
#include <stdint.h>

char Name[] = "Safin Rashid";
char EID[] = "srr3288";

/*
Starting UART:
x0=457, x1=-100, y1=-50000, x2=100, y2=50000

Brief description of the Lab: 
An embedded system is capturing data from a
sensor and performing analysis on the captured data.
The three analysis subroutines are:
  1. Calculate the average error
  2. Perform a linear equation using integer math 
  3. Check if the captured readings are a monotonic series
*/

// Function 1: Average Error
int32_t AverageError(const int32_t Readings[], const uint32_t N){
	
	
	int32_t AveErrorSum = 0, AveErr = 0, tempsum = 0, ReadingNum = 0, x0 = 457, Nn = N;
	
	while ( Nn > 0 ){
		tempsum = abs(Readings[ReadingNum] - x0);
		ReadingNum++;
		AveErrorSum += tempsum;
		Nn--;
	}
	
	AveErr = AveErrorSum / N;
	
   return AveErr;
}

// Function 2: Linear Equation
int32_t Linear(int32_t const x){
	
	int32_t x1 = -100, y1 = -50000, x2 = 100, y2 = 50000, y = 0, m = 0, b = 0;
	
	m = (y2 - y1) / (x2 - x1);
	b = y1 - (m * x1);
	
	y = (m * x) + b;
	
  return y;
}

// Function 3: Monotonicity
int IsMonotonic(int32_t const Readings[], uint32_t const N){
	//UART: true if non-decreasing
	
	int32_t truefalse = 1, ReadingNum = 0, Nn = N, test;
	
	while ( Nn > 1 ){
		test = Readings[ReadingNum + 1] - Readings[ReadingNum];
		Nn--;
		if (test < 0){
		Nn = 0;
		truefalse = 0;
		}
		ReadingNum++;

	}
	
  return truefalse;
}


