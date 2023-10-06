;****************** Debug.s ***************
; Program written by: Safin, Alexa
; Date Created: 2/14/2017
; Last Modified: 1/10/2022

; You may assume your debug functions have exclusive access to SysTick
; However, please make your PortF initialization/access friendly,
; because you have exclusive access to only one of the PortF pins.

; Your Debug_Init should initialize all your debug functionality
; Everyone writes the same version of Debug_ElapsedTime
; Everyone writes Debug_Beat, but the pin to toggle is revealed in the UART window
; There are four possible versions of Debug_Dump. 
; Which version you implement is revealed in the UART window

;N=174
;Option 1: Histogram of the number each value as occurred
;Implement Hearbeat on PF2



; ****************Option 1******************
; Input: R0 7-bit strategic information 
; Output: none

; If R0 bit 6 is low, 
; - observe the value in bits 5-0 of R0 (value from 0 to 63): 
;     maintain a histogram recording the number of times each value as occurred
;     since N will be less than 200, no histogram count can exceed the 8-bit 255 maximum,  

; If R0 bit 6 is high,
; - Do nothing


SYSCTL_RCGCGPIO_R  EQU 0x400FE608
GPIO_PORTF_DATA_R  EQU 0x400253FC
GPIO_PORTF_DIR_R   EQU 0x40025400
GPIO_PORTF_DEN_R   EQU 0x4002551C
SYSCTL_RCGCTIMER_R EQU 0x400FE604
TIMER2_CFG_R       EQU 0x40032000
TIMER2_TAMR_R      EQU 0x40032004
TIMER2_CTL_R       EQU 0x4003200C
TIMER2_IMR_R       EQU 0x40032018
TIMER2_TAILR_R     EQU 0x40032028
TIMER2_TAPR_R      EQU 0x40032038
TIMER2_TAR_R       EQU 0x40032048

; RAM Area
            AREA    DATA, ALIGN=2
;place your debug variables in RAM here
            EXPORT DumpBuf
            EXPORT Histogram
            EXPORT MinimumTime
            EXPORT MaximumTime         
DumpBuf     SPACE 200 ; 200 8-bit I/O values, your N will be less than 200
Histogram   SPACE 64  ; count of the number of times each value has occured
MinimumTime SPACE 4   ; smallest elapsed time between called to Debug_ElapsedTime
MaximumTime SPACE 4   ; largest elapsed time between called to Debug_ElapsedTime

; you will need additional globals, but do not change the above definitions
LastTime	SPACE 4
NStore1		SPACE 4 ;dump
NStore2		SPACE 4	;elapsed time
Delay		SPACE 4

; ROM Area
        EXPORT Debug_Init
        EXPORT Debug_Dump 
        EXPORT Debug_ElapsedTime
        EXPORT Debug_Beat
;-UUU-Import routine(s) from other assembly files (like SysTick.s) here
        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
        EXPORT EID1
EID1    DCB "srr3288",0  ;replace ABC123 with your EID
        EXPORT EID2
EID2    DCB "av36756",0  ;replace ABC123 with your EID

		

;---------------Your code for Lab 4----------------
;Debug initialization for all your debug routines
;This is called once by the Lab4 grader to assign points (if you pass #2 into TExaS_Init)
;It is later called at the beginning of the main.s
;for options 1 and 3, place 0 into each element of Histogram
; save all registers (not just R4-R11)
; you will need to initialize global variables, Timer2 and Port F here
Debug_Init 

		PUSH {R0-R12,LR}
		
		;Port F and PF2 initialization
		 LDR R0, =SYSCTL_RCGCGPIO_R
		 LDR R1, [R0]
		 ORR R1, #0x20 ;F
		 STR R1, [R0]
		 NOP
		 NOP
		 
		 LDR R0, =GPIO_PORTF_DIR_R
		 LDR R1, [R0]
		 ORR R1, #0x04	;PF2
		 STR R1, [R0]	 
		 
		 LDR R0, =GPIO_PORTF_DEN_R
		 LDR R1, [R0]
		 ORR R1, #0x04	;PF2
		 STR R1, [R0]

		BL   Timer2_Init ;TIMER2_TAR_R is 32-bit down counter, modifies R0-R1

		LDR R0, =MinimumTime
		LDR R2, =0xFFFFFFFF
		STR R2, [R0]		;Min = big initial
		LDR R1, =MaximumTime
		LDR R3, =0
		STR R3, [R1]		;Max = small initial
		LDR R0, =LastTime	;set initial last time 0
		STR R3, [R0]
		LDR R0, =Delay
		LDR R1, =200 ;count
		STR R1, [R0]		;set delay 200
		
		MOV R2, #174 
		LDR R3, =NStore1	;counter dump
		STR R2, [R3]
		LDR R4, =NStore2	;counter elapsed_time
		STR R2, [R4]
		
		LDR R4, =Histogram	;initialize histogram
		MOV R5, #0x00 		;content	
		MOV R6, #0 			;index
sloop
		STRB R5, [R4, R6]
		ADD R6, #1			;index each 
		CMP R6, #64			;Histogram size=64
		BNE sloop
      
      POP {R0-R12,PC}
    
; There are four possible options: 0,1,2 or 3
; Debug_Dump is called after every output.
; Stop recording and measuring after N observations
; Don't stop after N calls to Debug_Dump, but stop after N changes to your data structures
; (N will be revealed to you in the UART window)
; Save all registers (not just R4-R11)

Debug_Dump
	PUSH {R0-R5,LR}
	
	  LDR R3, =NStore1	;load N counter
	  LDR R4, [R3]		
	  CMP R4, #0		;check to see if N is 0, if so dont store
	  BLE nodump_N
						 ;examine R0
	  ANDS R1, R0, #0x40 ;bit 6
	  BNE nodump_bit6
	  ANDS R0, R0, #0x3F ;bits 0-5 considered
	  
	  LDR R1, =Histogram	;store in histogram
	  LDRB R2, [R1, R0]		;find index, pull contents
	  ADD R2, #1			;add 1 to content
	  STRB R2, [R1, R0]		;store back
	  
	  SUBS R4, R4, #1	;decrement N counter
	  STR R4, [R3]		;store counter for next call
	  
;you write this
;assume capture is called about every 2.5ms (real board)
;Let M = number of instructions in your Debug_Dump
;Calculate T = M instructions * 2cycles/instruction * 12.5ns/cycle 
;13 instructions
;Calculate intrusiveness is T/2.5ms = ???
nodump_bit6
nodump_N
      POP {R0-R5,PC}



; Your Debug_ElapsedTime is called after every output.
; Input: none 
; Output: none
; - observe the current time as a 32-bit unsigned integer: 
;     NowTime = TIMER2_TAR_R
; - Starting with the second call you will be able to measure elapsed time:
;     calcalate ElapsedTime = LastTime-NowTime (down counter)
;     determine the Minimum and Maximum ElapsedTime
; - Set LastTime = NowTime (value needed for next call)
; - Stop recording after N calls (N revealed to you in the UART window)
; save all registers (not just R4-R11)
Debug_ElapsedTime      
		PUSH {R0-R11,LR}
		
		LDR R3, =NStore2 ;load N counter
		LDR R6, [R3]
		CMP R6, #0		;check to see if N is 0, if so dont store
		BEQ stoptime_N
		
		LDR R0, =LastTime	;fetch lasttime
		LDR R5, [R0]
		CMP R5, #0	
		BEQ jump  ;no LastTime(0) if first call
		
		LDR R1, =TIMER2_TAR_R ;NowTime			;not getting corrent time
		LDR R1, [R1]	;contents of NowTime
		SUB R2, R5, R1  ;elapsed time
		
		LDR R3, =MinimumTime ;check if min
		LDR R7, [R3]
		CMP R2, R7   ;R3=elapsed-min
		BLO ReplaceMin  ;if negative, change min
max		
		LDR R4, =MaximumTime ;check if max
		LDR R8, [R4]
		CMP R2, R8	;R4=elapsed-max
		BHI ReplaceMax  ;if positive, change max
		B jump
		
ReplaceMin
	;R3 = mintime
		STR R2, [R3]	;update min
		B max
		
ReplaceMax
	;R4 = maxtime
		STR R2, [R4]	;update max
		
jump
		LDR R1, =TIMER2_TAR_R ;NowTime
		LDR R1, [R1]
		STR R1, [R0] ;New LastTime = NowTime
	
		SUBS R6, R6, #1 ;decrement N counter
		LDR R3, =NStore2
		STR R6, [R3]	;store counter for next call
stoptime_N	

		POP {R0-R11,PC}
	
	
; Your Debug_Beat function is called every time through the main loop to
; indicate to the operator if the main program is running (not stuck or dead).
; Inputs: none
; Outputs: none
; However, slow down the flashing so the LED flashes at about 1 Hz. 
; 1Hz means repeating: high for 500ms, low for 500ms
; Basically, toggle an LED every Mth call to your Debug_Beat 
; Find the constant M, so the flashing rate is between 0.5 and 2 Hz.
; The Port F pin you need to use will be revealed to you in the UART window.
; Save all registers (not AAPCS) 

Debug_Beat
      PUSH {R0-R2,LR}
	
		LDR R0, =Delay
		LDR R1, [R0]
		CMP R1, #0	;decrement delay counter
		BEQ Toggle	;if 0, toggle LED
		B Count
Toggle		
		LDR R0, =GPIO_PORTF_DATA_R
		LDR R2, [R0]
		EOR R2, #0x04	;toggle
		STR R2, [R0]
		LDR R0, =200 	;reset counter
		LDR R1, =Delay 
		STR R0, [R1]	;store new counter
		B jumpcount
Count
		SUB R1, R1, #1	
		STR R1, [R0]	;decrement if no toggle
		
jumpcount

      POP  {R0-R2,PC}



;------------Timer2_Init------------
; This subroutine is functional and does not need editing
; Initialize Timer2 running at bus clock.
; Make it so TIMER2_TAR can be used as a 32-bit time
; TIMER2_TAR counts down continuously
; Input: none
; Output: none
; Modifies: R0,R1
Timer2_Init
    LDR R1,=SYSCTL_RCGCTIMER_R
    LDR R0,[R1]
    ORR R0,R0,#0x04
    STR R0,[R1]    ; activate TIMER2
    NOP
    NOP
    LDR R1,=TIMER2_CTL_R
    MOV R0,#0x00
    STR R0,[R1]    ; disable TIMER2A during setup
    LDR R1,=TIMER2_CFG_R
    STR R0,[R1]    ; configure for 32-bit mode
    LDR R1,=TIMER2_TAMR_R
    MOV R0,#0x02
    STR R0,[R1]    ; configure for periodic mode, default down-count settings
    LDR R1,=TIMER2_TAILR_R
    LDR R0,=0xFFFFFFFE
    STR R0,[R1]    ; reload value
    LDR R1,=TIMER2_TAPR_R
    MOV R0,#0x00
    STR R0,[R1]    ; no prescale, bus clock resolution
    LDR R1,=TIMER2_IMR_R
    MOV R0,#0x00
    STR R0,[R1]    ; no interrupts
    LDR R1,=TIMER2_CTL_R
    MOV R0,#0x01
    STR R0,[R1]    ; enable TIMER2A
    BX  LR          
  
	
    ALIGN      ; make sure the end of this section is aligned
    END        ; end of file


