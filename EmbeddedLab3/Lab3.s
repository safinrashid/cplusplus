;****************** Lab3.s ***************
; Program written by: Safin, Alexa
; Date Created: 2/4/2017
; Last Modified: 2/13/2022
; Brief description of the program
;   The LED toggles at 2 Hz and a varying duty-cycle
; Hardware connections (External: Two buttons and one LED)
;  Change is Button input  (1 means pressed, 0 means not pressed)
;  Breathe is Button input  (1 means pressed, 0 means not pressed)
;  LED is an output (1 activates external LED)
; Overall functionality of this system is to operate like this
;   1) Make LED an output and make Change and Breathe inputs.
;   2) The system starts with the the LED toggling at 2Hz,
;      which is 2 times per second with a duty-cycle of 30%.
;      Therefore, the LED is ON for 150ms and off for 350 ms.
;   3) When the Change button is pressed-and-released increase
;      the duty cycle by 20% (modulo 100%). Therefore for each
;      press-and-release the duty cycle changes from 30% to 70% to 70%
;      to 90% to 10% to 30% so on
;   4) Implement a "breathing LED" when Breathe Switch is pressed:
; PortE device registers
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608

        IMPORT  TExaS_Init
        THUMB
        AREA    DATA, ALIGN=2
;global variables go here

       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID1
EID1   DCB "av36756",0  ;replace ABC123 with your EID
       EXPORT EID2
EID2   DCB "srr3288",0  ;replace ABC123 with your EID
       ALIGN 4

     EXPORT  Start

Start
; TExaS_Init sets bus clock at 80 MHz, interrupts, ADC1, TIMER3, TIMER5, and UART0
     MOV R0,#2  ;0 for TExaS oscilloscope, 1 for PORTE logic analyzer, 2 for Lab3 grader, 3 for none
     BL  TExaS_Init ;enables interrupts, prints the pin selections based on EID1 EID2
 ; Your Initialization goes here
	 LDR R0, =SYSCTL_RCGCGPIO_R
	 LDR R1, [R0]
	 ORR R1, #0x10
	 STR R1, [R0]
	 NOP
	 NOP
	 
	 LDR R0, =GPIO_PORTE_DIR_R
	 LDR R1, [R0]
	 ORR R1, #0x020
	 BIC R1, #0x06
	 STR R1, [R0]
	 
	 LDR R0, =GPIO_PORTE_DEN_R
	 LDR R1, [R0]
	 ORR R1, #0x26
	 STR R1, [R0]
	 
	 LDR R0, =GPIO_PORTE_DATA_R


Loop  
; main engine goes here
	 MOV R4, #3
Repeat 	 
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BNE BreatheTen

	 LDR R1, [R0] 
	 ANDS R1, R1, #2
	 BNE Next
Here	 
	 CMP R4, #3
	 BEQ Thirty	
	 CMP R4, #5
	 BEQ Fifty	
	 CMP R4, #7
	 BEQ Seventy	
	 CMP R4, #9
	 BEQ Ninety
	 CMP R4, #1
	 BEQ Ten	
	 
Next 
	 ADD R4, R4, #2
	 CMP R4, #11
	 BEQ Back
	 B Hold
Back 
	 MOV R4, #1
	 B Hold
Hold 
	 LDR R1, [R0] 
	 ANDS R1, R1, #2
	 BEQ Here
	 B Hold
	 
	 
Thirty 	 
	 LDR R6, = 3000000
	 LDR R7, = 7000000
	 BL LED	 
	 B Repeat 
Fifty	 
	 LDR R6, = 5000000
	 LDR R7, = 5000000
	 BL LED	 
	 B Repeat  	 
Seventy	 
	 LDR R6, = 7000000
	 LDR R7, = 3000000
	 BL LED	 
	 B Repeat	 
Ninety	 
	 LDR R6, = 9000000
	 LDR R7, = 1000000
	 BL LED
	 B Repeat
Ten
	 LDR R6, = 1000000
	 LDR R7, = 9000000
	 BL LED	 
	 B Repeat 
	 
	 
	 
	 
;BREATHE CODE STARTS HERE

BreatheTen
	 MOV R8, #0
	 MOV R9, #125 ;time
DoneTen
	 LDR R6, = 1000
	 LDR R7, = 9000
	 BL LED	 
	 SUBS R9, R9, #1
	 BNE DoneTen
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 CMP R8, #0
	 BEQ BreatheTwenty
	 B BreatheNinety

BreatheTwenty
	 MOV R9, #125 ;time
DoneTwenty
	 LDR R6, = 2000
	 LDR R7, = 8000
	 BL LED	
	 SUBS R9, R9, #1
	 BNE DoneTwenty	 
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 CMP R8, #0
	 BEQ BreatheThirty
	 B BreatheTen
	 
BreatheThirty 	 
	 MOV R9, #125 ;time
DoneThirty
	 LDR R6, = 3000
	 LDR R7, = 7000
	 BL LED	
	 SUBS R9, R9, #1
	 BNE DoneThirty 
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 CMP R8, #0
	 BEQ BreatheForty
	 B BreatheTwenty

BreatheForty 	 
	 MOV R9, #125 ;time
DoneForty
	 LDR R6, = 4000
	 LDR R7, = 6000
	 BL LED	
	 SUBS R9, R9, #1
	 BNE DoneForty	  
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 CMP R8, #0
	 BEQ BreatheFifty
	 B BreatheThirty

BreatheFifty	 
	 MOV R9, #125 ;time
DoneFifty
	 LDR R6, = 5000
	 LDR R7, = 5000
	 BL LED	
	 SUBS R9, R9, #1
	 BNE DoneFifty 	 
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 CMP R8, #0
	 BEQ BreatheSixty
	 B BreatheForty
	 
BreatheSixty 	 
	 MOV R9, #125 ;time
DoneSixty
	 LDR R6, = 6000
	 LDR R7, = 4000
	 BL LED	
	 SUBS R9, R9, #1
	 BNE DoneSixty  
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 CMP R8, #0
	 BEQ BreatheSeventy
	 B BreatheFifty 

BreatheSeventy	 
	 MOV R9, #125 ;time
DoneSeventy
	 LDR R6, = 7000
	 LDR R7, = 3000
	 BL LED	
	 SUBS R9, R9, #1
	 BNE DoneSeventy	  
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 CMP R8, #0
	 BEQ BreatheEighty
	 B BreatheSixty
	 
BreatheEighty 	 
	 MOV R9, #125 ;time
DoneEighty
	 LDR R6, = 8000
	 LDR R7, = 2000
	 BL LED	
	 SUBS R9, R9, #1
	 BNE DoneEighty 
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 CMP R8, #0
	 BEQ BreatheNinety
	 B BreatheSeventy
	 
BreatheNinety	 
	 MOV R9, #125 ;time
DoneNinety
	 LDR R6, = 9000
	 LDR R7, = 1000
	 BL LED
	 SUBS R9, R9, #1
	 BNE DoneNinety	 
	 LDR R1, [R0] 
	 ANDS R1, R1, #4
	 BEQ Loop
	 MOV R8, #1
	 B BreatheEighty
 


;FLASH LED SUBROUTINE

LED
	LDR R1, [R0]		
	ORR R1, #0x20
	STR R1, [R0]
	ADD R2, R6, #0
DELAYNMS_ON
	SUBS R2, #1
	BNE DELAYNMS_ON
	BIC R1, #0x20
	ADD R2, R7, #0
	STR R1, [R0]
DELAYNMS_OFF
	SUBS R2, #1
	BNE DELAYNMS_OFF	
	BX LR


     ALIGN      ; make sure the end of this section is aligned
     END        ; end of file
