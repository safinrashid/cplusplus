;****************** Lab1.s ***************
; Program initially written by: Yerraballi and Valvano
; Author: Safin Rashid
; Date Created: 1/15/2018 
; Last Modified: 1/29/2022
; Brief description of the program: Solution to Lab1
; The objective of this system is to implement a parity system
; Hardware connections: 
;  One output is positive logic, 1 turns on the LED, 0 turns off the LED
;  Three inputs are positive logic, meaning switch not pressed is 0, pressed is 1

;Lab 1, Spring 2022, EID=SRR3288
;Implement odd parity using switches on PD2,PD1,PD0 and LED on PD5


GPIO_PORTD_DATA_R  EQU 0x400073FC
GPIO_PORTD_DIR_R   EQU 0x40007400
GPIO_PORTD_DEN_R   EQU 0x4000751C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
	
       PRESERVE8 
       AREA   Data, ALIGN=4
; No global variables needed

       ALIGN 4
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID
EID    DCB "srr3288",0  ;replace abc123 with your EID
       EXPORT RunGrader
	   ALIGN 4
RunGrader DCD 1 ; change to nonzero when ready for grading
           
      EXPORT  Lab1
Lab1 
;Initializations
;Turn clock on for port D
	LDR R0, =SYSCTL_RCGCGPIO_R	;load address
	LDR R1, [R0]	;load contents
	ORR R1, #0x08	;bit 8=pin D
	STR R1, [R0]	;store back
	NOP
	NOP				;run 2 clock cycles to stabilize
;Set PD5 to be output
	LDR R0, =GPIO_PORTD_DIR_R
	LDR R1, [R0]
	ORR R1, #0x20	;00100000 pin 5=1
	STR R1, [R0]
;Digital enable PD0,1,2,5   
	LDR R0, =GPIO_PORTD_DEN_R
	LDR R1, [R0]
	ORR R1, #0x27 ;00100111 pins 5,2,1,0=1
	STR R1, [R0]
	
	LDR R6, =GPIO_PORTD_DATA_R	;load data register address for port D


loop
;input, calculate, output 
;PD2 EOR PD1 EOR PD0 EOR 1 = PD5
	
	LDR R0, [R6]
	LDR R1, [R6]
	LDR R2, [R6]	;load contents of port D data register
	
	AND R0, #1		;PD0 ;00000001
	AND R1, #2		;PD1 ;00000010
	LSR R1, R1, #1		 ;00000001 right shift 1
	AND R2, #4		;PD2 ;00000100
	LSR R2, R2, #2		 ;00000001 right shift 2
	MOV R3, #0x01	;equivalent eor(1)
	
	EOR R1, R0, R1	;PD0 xor PD1
	EOR R2, R1, R2	; ^ xor PD2
	EOR R3, R2, R3	; ^ xor x01
	LSL R5, R3, #5	;result in PD5(bit 5)
	STR R5, [R6]	;store into Port D data register
	
    B    loop


    ALIGN        ; make sure the end of this section is aligned
    END          ; end of file