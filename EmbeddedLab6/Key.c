#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// **************Key_Init*********************
// Initialize piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: none
void Key_Init(void){ volatile uint32_t delay;
	
		SYSCTL_RCGC2_R |= 0x3F; //clock on ports A and B and F
		while((SYSCTL_RCGCGPIO_R&0x00000002)==0){}; //wait for clock (= NOP x2)
	
	  GPIO_PORTE_DIR_R &= ~0x01;		//inputs on PA5-2 (clear)
		GPIO_PORTE_DEN_R |= 0x01;			//enable digital on PA2-5
			
		GPIO_PORTF_DIR_R |= 0x0E;		//inputs on PA5-2 (clear)
		GPIO_PORTF_DEN_R |= 0x0E;		//enable digital on PA2-5

}


// **************Key_In*********************
// Input from piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
uint32_t Key_In(void){ 
  
	return (GPIO_PORTE_DATA_R & 0x01)<<1;
	  
}

//------------LaunchPad_Init------------
// Initialize Switch input and LED output
// Input: none
// Output: none
void LaunchPad_Init(void){
// implement if needed
}
//------------LaunchPad_Input------------
// Input from Switches, 
// Convert hardware negative logic to software positive logic 
// Input: none
// Output: 0x00 none
//         0x01 SW2 pressed (from PF4)
//         0x02 SW1 pressed (from PF1)
//         0x03 both SW1 and SW2 pressed
uint8_t LaunchPad_Input(void){
// implement if needed
  return 0; // replace   
}
//------------LaunchPad__Output------------
// Output to LaunchPad LEDs 
// Positive logic hardware and positive logic software
// Input: 0 off, bit0=red,bit1=blue,bit2=green
// Output: none
void LaunchPad_Output(uint8_t data){  // write three outputs bits of PORTF
// implement if needed

}
