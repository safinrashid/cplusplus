// Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
// When all inputs true, ... South, Walk, West, South, Walk, West, South, Walk, West, ...

// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south red light connected to bit 2
// north/south yellow light connected to bit 1
// north/south green light connected to bit 0

// pedestrian detector connected to most significant bit(4) (1=pedestrian present)
// north/south car detector connected to middle bit(2) (1=car present)
// east/west car detector connected to least significant bit(1) (1=car present)
// "walk" light connected to PF321 (built-in white LED)
// "don't walk" light connected to PF1 (built-in red LED)

#include <stdint.h>
#include "SysTick.h"
#include "Lab5grader.h"
#include "../inc/tm4c123gh6pm.h"
// put both EIDs in the next two lines
char EID1[] = "srr3288"; //  ;replace abc123 with your EID
char EID2[] = "av36756"; //  ;replace abc123 with your EID

void DisableInterrupts(void);
void EnableInterrupts(void);

struct state {
  unsigned long OutputB;		
	unsigned long OutputF;		
  unsigned long Wait;  			
  unsigned long NextState[8];
};
	
typedef struct state state_t;

#define goS  			0		//green South
#define goW  			1		//green West
#define StoW  		2		//South yellow to West
#define WtoS  		3		//West yellow to South
#define StoP  		4		//South yellow to walk
#define WtoP  		5		//West yellow to walk
#define walk  		6		//walk, all red
#define wlk_r1    7		//flash1 red
#define wlk_o1		8		//flash1 off
#define wlk_r2		9		//flash2 red
#define wlk_o2		10	//flash2 off
#define allstop_S	11	//all red before South
#define allstop_W	12	//all red before West
#define allstop_P	13	//all red before walk

#define greentime 	20 //x10ms
#define yellowtime 	5
#define walktime 		10
#define flashtime		5
#define redtime			7

state_t FSM[14]={ //14 states	
//in:												0						1		 				2			  		 3					 4					5				 		6			 				7
	//outB outF  wait       none	      west	      south		      s+w			  walk/p			  p+w			   p+s		    	all (S, wk, W ...)
	{0x21, 0x2,  greentime, 	{			 goS, 			 StoW, 				goS, 			 StoW, 			 StoP, 			 StoP, 			 StoP, 			 StoP}},	//goS
	{0x0C, 0x2,  greentime, 	{			 goW,					goW, 			 WtoS, 			 WtoS, 			 WtoP, 			 WtoP, 			 WtoS, 			 WtoS}},	//goW
	
	{0x22, 0x2,  yellowtime,  {allstop_W, 	allstop_W, 	allstop_W, 	allstop_W, 	allstop_W, 	allstop_W, 	allstop_W, 	allstop_W}},	//StoW//
	{0x14, 0x2,  yellowtime,  {allstop_S, 	allstop_S, 	allstop_S, 	allstop_S, 	allstop_S, 	allstop_S, 	allstop_S, 	allstop_S}},	//WtoS//
	{0x22, 0x2,  yellowtime,  {allstop_P, 	allstop_P, 	allstop_P, 	allstop_P, 	allstop_P, 	allstop_P, 	allstop_P,	allstop_P}},	//StoP//
	{0x14, 0x2,  yellowtime,  {allstop_P, 	allstop_P, 	allstop_P, 	allstop_P, 	allstop_P, 	allstop_P, 	allstop_P, 	allstop_P}},	//WtoP//
	
	{0x24, 0xE,  	 walktime, 	{			walk, 		 wlk_r1, 		 wlk_r1, 		 wlk_r1, 			 walk, 		 wlk_r1, 		 wlk_r1, 		 wlk_r1}},	//walk
	{0x24, 0x2,  	flashtime, 	{	  wlk_o1, 		 wlk_o1, 		 wlk_o1, 		 wlk_o1, 			 walk, 		 wlk_o1, 		 wlk_o1, 		 wlk_o1}},	//wlk_r1//
	{0x24, 0x0,  	flashtime, 	{	  wlk_r2, 		 wlk_r2, 		 wlk_r2, 		 wlk_r2, 			 walk, 		 wlk_r2, 		 wlk_r2, 		 wlk_r2}},	//wlk_o1//
	{0x24, 0x2,  	flashtime, 	{	  wlk_o2, 		 wlk_o2, 		 wlk_o2, 		 wlk_o2, 			 walk, 		 wlk_o2, 		 wlk_o2, 		 wlk_o2}},	//wlk_r2//
	{0x24, 0x0,  	flashtime, 	{  		walk, 	allstop_W, 	allstop_S, 	allstop_W,			 walk, 	allstop_W,	allstop_S, 	allstop_W}},	//wlk_o2// can replace
	
	{0x24, 0x2,   	redtime,	{			 goS, 				goS, 				goS, 				goS, 				goS, 				goS, 				goS, 				goS}},	//allstop_S//
	{0x24, 0x2,   	redtime,  {			 goW, 				goW, 				goW, 				goW, 				goW, 				goW, 				goW, 				goW}},	//allstop_W//
	{0x24, 0x2,   	redtime,  {			walk, 			 walk,			 walk, 			 walk, 			 walk, 			 walk,			 walk, 			 walk}},	//allstop_P//
};


int main(void){ 
		
  DisableInterrupts();
	
  TExaS_Init(5);
  SysTick_Init();   // Initialize SysTick for software waits
	
	uint32_t delay, In;
	
	//PA: input, PB: traffic lights, PF: walk light
	
  SYSCTL_RCGC2_R |= 0x23;											//clock on ports A, B, F
  delay = SYSCTL_RCGC2_R;											//delay
	
	//PORT INTIALIZATIONS
  GPIO_PORTA_DIR_R &= ~0x1C;									//inputs on PA3-5 (clear)
  GPIO_PORTA_DEN_R |= 0x1C;										//enable digital on PA3-5

  GPIO_PORTB_DIR_R |= 0x3F;										//output on PB0-5 (set)
  GPIO_PORTB_DEN_R |= 0x3F;										//enable digital on PB0-5

  GPIO_PORTF_DIR_R |= 0x0E;										//output on PF123 (set)
  GPIO_PORTF_DEN_R |= 0x0E;										//enable digital on PF1-PF3

	uint8_t CS = goS;	//initial state

  EnableInterrupts(); 
	
	//0011 1000
  while(1){		//always repeat

		GPIO_PORTB_DATA_R = FSM[CS].OutputB;	//produce output based on CS for traffic light (PA)
		GPIO_PORTF_DATA_R = FSM[CS].OutputF;	//produce output based on CS for walk light (PF)
		
    SysTick_Wait10ms(FSM[CS].Wait);				//wait based on CS 
		
    In = (GPIO_PORTA_DATA_R & 0x1C) >> 2;	//read input, isolate pins, shift rightmost
		CS = FSM[CS].NextState[In];						//change CS based on CS and input (0-7)

  }
}

