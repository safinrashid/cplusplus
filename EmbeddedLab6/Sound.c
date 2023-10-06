// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "../inc/tm4c123gh6pm.h"

const uint8_t wave [64] = {32,35,38,41,44,47,49,52,54,56,58,
	59,61,62,62,63,63,63,62,62,61,59,58,56,54,52,49,47,44,41,38,
	35,32,29,26,23,20,17,15,12,10,8,6,5,3,2,2,1,1,1,2,2,3,5,6,8,
	10,12,15,17,20,23,26,29};
uint32_t Index = 0;

// **************Sound_Init*********************
// Initialize digital outputs and SysTick timer
// Called once, with sound/interrupts initially off
// Input: none
// Output: none

void Sound_Init(void){
	
	DAC_Init();

	//Initialize SysTick
	NVIC_ST_CTRL_R = 0;																						// 1) Diasble SysTick during setup
	NVIC_ST_RELOAD_R = 0;																					// 2) Setup time between each interupt
	NVIC_ST_CURRENT_R = 0;																				// 3) Write any value to current to clear it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;		// Set Priority 3

}

// **************Sound_Start*********************
// Start sound output, and set Systick interrupt period 
// Sound continues until Sound_Start called again, or Sound_Off is called
// This function returns right away and sound is produced using a periodic interrupt
// Input: interrupt period
//           Units of period to be determined by YOU
//           Maximum period to be determined by YOU
//           Minimum period to be determined by YOU
//         if period equals zero, disable sound output
// Output: none
void Sound_Start(uint32_t period){

	NVIC_ST_CTRL_R |= 0x02;		// 1) Enable SysTick 
	NVIC_ST_RELOAD_R = period;	//Key0-Key3
	NVIC_ST_CTRL_R = 0x0007; //enable systick w core clock and interrupts	

		
}

// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
 
	NVIC_ST_CTRL_R = 0;

}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
	
	DAC_Out(wave[Index]);						//Output digital values to DAC every period 
	Index =(Index+1)%64;

}


void Sound_Voice(const uint8_t *voice){
  //optional
	// **************Sound_Voice*********************
// Change voice
// EE319K optional
// Input: voice specifies which waveform to play
//           Pointer to wave table
// Output: none


}


const uint8_t *Sound_GetVoice(void){
#define PF4 (*((volatile uint32_t *)0x40025040))
#define PF3 (*((volatile uint32_t *)0x40025020))
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF1 (*((volatile uint32_t *)0x40025008))
#define PF0 (*((volatile uint32_t *)0x40025004))
  return 0; // replace
	//optional
	// **************Sound_GetVoice*********************
// Read the current voice
// EE319K optional
// Input: 
// Output: voice specifies which waveform to play
//           Pointer to current wavetable
}
