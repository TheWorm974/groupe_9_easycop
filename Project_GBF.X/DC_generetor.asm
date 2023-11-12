#include<p18F4550.inc>
CONFIG WDT=OFF
CONFIG MCLRE=ON
CONFIG DEBUG=OFF
CONFIG FOSC=INTOSC_EC
    
org 0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program
    
org 0x0008            
    GOTO    INT_ADC  


START   ; let linker place main program
 
 clrf TRISD
 clrf PORTD
 clrf PORTA
 
 movlw B'00001110'
 movwf ADCON1
 movlw B'00000000'
 movwf ADCON0
 bsf ADCON0, ADON
 bsf INTCON, GIE
 bsf INTCON, PEIE
 bsf PIE1, ADIE

Main
 
 bsf ADCON0, GO_DONE
 GOTO Main                          ; loop forever
    
INT_ADC
    
    btfsc PIR1, ADIF
    goto AD_interrupt
    retfie
    
AD_interrupt
    
    bcf PIR1, ADIF
    
    movff ADRESH, PORTD
    retfie

END