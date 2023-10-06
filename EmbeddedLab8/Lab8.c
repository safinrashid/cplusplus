// Lab8.c
// Runs on TM4C123
// Student names: Safin, Alexa
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 1/12/2021 

// Specifications:
// Measure distance using slide pot, sample at 10 Hz
// maximum distance can be any value from 1.5 to 2cm
// minimum distance is 0 cm
// Calculate distance in fixed point, 0.001cm
// Analog Input connected to PD2=ADC0 channel 5
// displays distance on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats (use them in creative ways)
// 

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "../inc/tm4c123gh6pm.h"

//*****the first four main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
//       used to test ADC and slide pot
// main2 adds the LCD to the ADC and slide pot, ADC data is on ST7735
//       used to measure execution time of ADC and LCD
// main3 adds your convert function, plot position on ST7735
//       used to test Nyquist Theorem
// main4 used to test Central Limit Theorem
//       observe noise versus ADC0_SAC_R

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

	
uint32_t data, position, mailbox, status = 1;
int32_t debug;

// your function to convert 12 bit ADC sample to distance (0.001cm)
// input: x is 12 bit ADC digital sample
// output: integer part of distance in 0.001 resolution
uint32_t Convert(uint32_t x){
	
	return (5109*x)/10000+25;
}


// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
  volatile int delay;
  SYSCTL_RCGCGPIO_R |= 0x20;
  __asm__{
		NOP
		NOP}
  GPIO_PORTF_DIR_R |= 0x0E;
  GPIO_PORTF_DEN_R |= 0x0E;
}
uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
int main1(void){      // single step this program and look at Data
  DisableInterrupts();
  TExaS_Init(SCOPE);  // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 5
  EnableInterrupts();
  while(1){                
    Data = ADC_In();  // sample 12-bit channel 5
  }
}
uint32_t startTime,stopTime;
uint32_t ADCtime,Converttime,OutDectime,OutFixtime; // in usec
int main2(void){
	
  TExaS_Init(SCOPE);  // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 5
  ST7735_InitR(INITR_REDTAB); 
  NVIC_ST_RELOAD_R = 0x00FFFFFF; // maximum reload value
  NVIC_ST_CURRENT_R = 0;    // any write to current clears it
  NVIC_ST_CTRL_R = 5;
  while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec           
    startTime= NVIC_ST_CURRENT_R;
    Data = ADC_In();  // sample 12-bit channel 5
    stopTime = NVIC_ST_CURRENT_R;
    ADCtime = ((startTime-stopTime)&0x0FFFFFF)/80; // usec

    ST7735_SetCursor(0,0);
    startTime= NVIC_ST_CURRENT_R;
    LCD_OutDec(Data); 
    ST7735_OutString("    ");  // spaces cover up characters from last output
    stopTime = NVIC_ST_CURRENT_R;
    OutDectime = ((startTime-stopTime)&0x0FFFFFF)/80; // usec
		LCD_OutFix(ADCtime);
		ST7735_OutString("    ");
		LCD_OutFix(OutDectime);
  }
}




int main3(void){ 
	uint32_t time=0;
  volatile uint32_t elapsedTime;
  uint32_t sampleStartTime;
  TExaS_Init(SCOPE);         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
  NVIC_ST_RELOAD_R = 0x00FFFFFF; // maximum reload value
  NVIC_ST_CURRENT_R = 0;    // any write to current clears it
  NVIC_ST_CTRL_R = 5;
  PortF_Init();
  ADC_Init();         // turn on ADC, set channel to 5
  ST7735_PlotClear(0,2000); 
  while(1){  // fs = 80,000,000/5,000,000 = 16 Hz
    sampleStartTime = NVIC_ST_CURRENT_R;
    GPIO_PORTF_DATA_R ^= 0x04;      // Heartbeat
    Data = ADC_In();  // sample 12-bit channel 5
    startTime= NVIC_ST_CURRENT_R;
    Position = Convert(Data); 
    stopTime = NVIC_ST_CURRENT_R;
    Converttime = ((startTime-stopTime)&0x0FFFFFF)/80; // usec

    ST7735_SetCursor(0,0);
    LCD_OutDec(Data); ST7735_OutString("    "); 
    startTime= NVIC_ST_CURRENT_R;
    ST7735_SetCursor(6,0);
    LCD_OutFix(Position);
    stopTime = NVIC_ST_CURRENT_R;
    OutFixtime = ((startTime-stopTime)&0x0FFFFFF)/80; // usec
    if((time%8)==0){
      ST7735_PlotPoint(Position);
      ST7735_PlotNextErase(); // data ploted at about 2 Hz
    }
    time++; // executed about every 62.5 ms
    do{
      elapsedTime = (sampleStartTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
    }
    while(elapsedTime <= 5000000);  // establishes approximate sampling rate
  }
} 

uint32_t Histogram[64]; // probability mass function
uint32_t Center;
// main4 program to study CLT, for honors
int main4(void){ 
	uint32_t i,d,sac;
  DisableInterrupts();
  TExaS_Init(SCOPE);    // Bus clock is 80 MHz 
  // Connect PF3 to PD3
  ST7735_InitR(INITR_REDTAB); 
  ADC_Init();        // turn on ADC, PD2, set channel to 5
  PortF_Init();
  EnableInterrupts();
  sac = 0;
  while(1){
    ADC0_SAC_R = sac;
    GPIO_PORTF_DATA_R ^= 0x08;       // Heartbeat
    Center = ADC_In();
    for(i=0; i<64; i++) Histogram[i] = 0; // clear
    for(i=0; i<200; i++){
      for(int j=0; j<1000 ;j++){};
      Data = ADC_In();
      GPIO_PORTF_DATA_R ^= 0x08;       // Heartbeat
      if(Data<Center-32){
         Histogram[0]++;
      }else if(Data>=Center+32){
         Histogram[63]++;
      }else{
        d = Data-Center+32;
        Histogram[d]++;
      }
    }
    ST7735_PlotClear(0,100);
    ST7735_SetCursor(0,0);
    ST7735_OutString("SAC = ");
    LCD_OutDec(sac);
    for(i=0; i<63; i++){
      if(Histogram[i]>99) Histogram[i]=99;
      ST7735_PlotBar(Histogram[i]);
      ST7735_PlotNext();
      ST7735_PlotBar(Histogram[i]);
      ST7735_PlotNext();
    }
    if(sac<6) sac++;
    else sac = 0;      
  }
}
















void SysTick_Init(uint32_t period){

	NVIC_ST_CTRL_R = 0;																						
	NVIC_ST_RELOAD_R = period-1;																	
	NVIC_ST_CURRENT_R = 0;																				
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;		
	NVIC_ST_CTRL_R = 0x0007; 
	//From lab6

}

void SysTick_Handler(void){
	
	GPIO_PORTF_DATA_R ^= 0x04;// Heartbeat

	//capture output from ADC (12-bit)
	data = ADC_In();
	mailbox = ADC_In();
	status = 1; //flag
	
}


int main(void){ // this is real lab 8 main
	
	TExaS_Init(SCOPE);
	ST7735_InitR(INITR_REDTAB);
	SysTick_Init(4000000);
	PortF_Init();
	ADC_Init();
	
	//main2();
	
	uint32_t out;
	EnableInterrupts();
  while(1){
    if (status == 1) {
			
			//main3();
			
			status = 0;
			//12-bit to CM
			out = Convert(mailbox);
			debug = Convert(mailbox); 
			if(out <= 217){
			out = 0; }
			
			//printing
			ST7735_SetCursor(0,0);
			LCD_OutFix(out);
			ST7735_OutString(" CM");
		}
  }
}

