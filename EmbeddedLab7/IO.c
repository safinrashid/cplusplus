// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on TM4C123
// Program written by: Safin, Alexa
// Date Created: March 30, 2018
// Last Modified:  change this or look silly
// Lab number: 7

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED (PF2 LED/out, PF4 switch/in)
// Input: none
// Output: none
void IO_Init(void) {
	
	SYSCTL_RCGC2_R |= 0x20; 		//clock on port F
	__asm("nop");__asm("nop");	//wait 2 clock cycles
				

	GPIO_PORTF_LOCK_R = 0x4C4F434B;//unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x1F; 		// allow changes to PF4-0
				
	GPIO_PORTF_AMSEL_R = 0x00; 	//disable analog on PF
	GPIO_PORTF_PCTL_R = 0x00000000;//PCTL GPIO on PF4-0

	GPIO_PORTF_DIR_R = 0x04;    //input PF4, output PF2
				
	GPIO_PORTF_AFSEL_R = 0x00; 	//disable alt funct on PF
	GPIO_PORTF_PUR_R = 0x10; 		//enable pull-up on PF4
				
	GPIO_PORTF_DEN_R |= 0x14;		//enable digital on PF2/4
	
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
 
	GPIO_PORTF_DATA_R ^= 0x04;	//toggle

}

// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
	volatile uint32_t time;
	while(n){
		time = 6665;  // 1msec, tuned at 80 MHz
		while(time){
			time--;
		}
		n--;
	}
}

//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
	while(GPIO_PORTF_DATA_R>>4 == 0){ 
	}//negative logic (press = 0)
	
	DelayMs(20);//20ms delay
	
	while(GPIO_PORTF_DATA_R>>4 == 1){
	}
	
}  

