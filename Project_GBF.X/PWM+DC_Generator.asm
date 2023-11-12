#include<p18F4550.inc>
CONFIG WDT=OFF
CONFIG MCLRE=ON
CONFIG DEBUG=OFF
CONFIG FOSC=INTOSC_EC ; Internal oscillator
    
org 0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program
    
org 0x0008            
    GOTO    INT_ADC  

START   ; let linker place main program

 count equ H'01'
 
 clrf TRISD
 clrf PORTD
 bcf TRISA, RA3
 bcf TRISA, RA2
 clrf PORTA
 
 movlw B'00001110'
 movwf ADCON1
 movlw B'00000000'
 movwf ADCON0
 bsf ADCON0, ADON
 bsf INTCON, GIE
 bsf INTCON, PEIE
 bsf PIE1, ADIE
    
 movlw B'01110010'
 movwf OSCCON ; Set internal clock to 8MHz
 bcf TRISC, RC2
 bsf TRISC, RC7
 bsf TRISC, RC0
 bsf TRISC, RC1
 setf TRISB
 clrf PORTC
 
 
 movlw B'00001100'
 movwf CCP1CON
 movlw B'00000010'
 movwf T2CON
 clrf TMR2
 goto Main
 
Main
 
 btfsc PORTB, RB0
 GOTO DC_Function
 btfsc PORTB, RB1
 GOTO PWM_Function
 
 GOTO Main 

DC_Function
 clrf PORTA
 bcf T2CON, TMR2ON
 bsf ADCON0, GO_DONE 
 btfsc PORTB, RB1
 goto PWM_Function
 goto DC_Function


PWM_Function
 clrf PORTA
 clrf PORTD
 bcf ADCON0, GO_DONE
 bsf T2CON, TMR2ON
 btfsc PORTB, RB6
 goto Frq_1
 btfsc PORTB, RB7
 goto Frq_2
 btfsc PORTB, RB0
 goto DC_Function
 bsf PORTA, RA3
 movlw B'11110011'
 movwf PORTD
 goto PWM_Function
 
Frq_1
 clrf PORTD
 clrf PORTA
 movlw B'01111101'
 movwf PR2
 btfsc PORTC, RC0
 CALL DUTY10_1
 btfsc PORTC, RC1
 CALL DUTY50_1
 btfsc PORTC, RC7
 CALL DUTY90_1
 btfsc PORTB, RB7
 goto Frq_2
 btfsc PORTB, RB0
 goto DC_Function
 bsf PORTA, RA3
 movlw B'10000110'
 movwf PORTD
 goto Frq_1
 
Frq_2
 clrf PORTD
 clrf PORTA
 movlw B'00001100'
 movwf PR2
 btfsc PORTC, RC0
 CALL DUTY10_2
 btfsc PORTC, RC1
 CALL DUTY50_2
 btfsc PORTC, RC7
 CALL DUTY90_2
 btfsc PORTB, RB6
 goto Frq_1
 btfsc PORTB, RB0
 goto DC_Function
 bsf PORTA, RA3
 movlw B'00000110'
 movwf PORTD
 call My_Delay
 bcf PORTA, RA3
 bsf PORTA, RA2
 movlw B'10111111'
 movwf PORTD
 call My_Delay
 bcf PORTA, RA2
 goto Frq_2

 
DUTY10_1
 movlw B'00001101'
 movwf CCPR1L
 retfie


DUTY50_1
 movlw B'00111111'
 movwf CCPR1L
 retfie

 
DUTY90_1
 movlw B'01110010'
 movwf CCPR1L
 retfie
 
DUTY10_2
 movlw B'00000001'
 movwf CCPR1L
 retfie


DUTY50_2
 movlw B'00000111'
 movwf CCPR1L
 retfie
 
 
DUTY90_2
 movlw B'00001011'
 movwf CCPR1L
 retfie
 
INT_ADC
    
    btfsc PIR1, ADIF
    goto AD_interrupt
    retfie
    
AD_interrupt
    
    bcf PIR1, ADIF
    
    movff ADRESH, PORTD
    retfie
    
My_Delay
    decfsz count
    goto My_Delay
    retfie
 
END