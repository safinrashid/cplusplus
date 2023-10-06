#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Music.h"
#include "Lab6Grader.h"
// put both EIDs in the next two lines
char EID1[] = "srr3288"; //  ;replace abc123 with your EID
char EID2[] = "av36756"; //  ;replace abc123 with your EID

#define Key3	523.3	// PA5
#define Key2  466.2	// PA4
#define Key1  392		// PA3
#define Key0	311.1	// PA2

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);
void DAC_Init(void);          // your lab 6 solution
void DAC_Out(uint8_t data);   // your lab 6 solution
uint8_t Testdata;

// lab video Lab6_voltmeter, Program 6.1
// A simple program that outputs sixteen DAC values. Use this main if you have a voltmeter.
const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};

int voltmetermain(void){ uint32_t i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  while(1){                
    Testdata = Inputs[i];
    DAC_Out(Testdata); // your lab 6 solution
    i=(i+1)&0x0F;  // <---put a breakpoint here
  }
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
// lab video Lab6_static. Program 6.2
// A simple program that outputs sixteen DAC values. 
// Use this main if you do not have a voltmeter. 
// Connect PD3 to your DACOUT and observe the voltage using TExaSdisplay in scope mode.
int staticmain(void){  
  uint32_t last,now,i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
      Testdata = Inputs[i];
      DAC_Out(Testdata); // your lab 6 solution
      i=(i+1)&0x0F;
    }
    last = now;
    DelayMs(25);   // debounces switch
  }
}
      


void heartbeat(uint8_t status){
	
	static uint32_t delay = 7000;
	uint32_t time = 7000;
	
	if(status == 1){
	if(delay == 0){
		GPIO_PORTF_DATA_R ^= 0x0E;
		delay = time; //reload 
	}else{
		delay--;
	}
}else{
	GPIO_PORTF_DATA_R = 0x0;
}
}



int main(void){       
  DisableInterrupts();
	
  TExaS_Init(SIMULATIONGRADER);    // bus clock at 80 MHz
  
	Key_Init();
  LaunchPad_Init();
  Sound_Init();
  Music_Init();
  EnableInterrupts();
		
  while(1){                
		uint32_t period;
		
		if (Key_In() == 1){
		  period = 4018;
			heartbeat(1);
		}
		else if (Key_In() == 2){
			period = 3188;
			heartbeat(1);
		}
		else if (Key_In()== 4){
			period = 2681;
			heartbeat(1);
		}		
		else if (Key_In() == 8){
			period = 2388;
			heartbeat(1);
		}		
		else if (Key_In() == 0){
			period = 0;
			Sound_Off();//if none or multiple
			heartbeat(0);
		}
		

		Sound_Start(period);
		WaitForInterrupt();
		
  }      
	
}

