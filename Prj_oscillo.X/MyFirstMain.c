/* 
 * File:   newmain.c
 * Author: theworm
 *
 * Created on 25 octobre 2023, 11:39
 */

#include<xc.h>
#include<stdio.h>
#include<stdlib.h>
#include"glcd.h" // library for glcd screen

#define _XTAL_FREQ 8000000
#pragma config WDT = OFF // watchdogOFF
#pragma config FOSC = HS // Oscillator


/* Just prototypes */
void initMyPIC18F(void);
void PORTB_Interrupt_ISR(void);
void __interrupt() irq_handle(void);
void toggle(void);
void MyHelloWorld(void);
/*
 * 
 */
int main(int argc, char** argv) {
    initMyPIC18F();
    PORTCbits.RC1 = 0;
    
   // MyHelloWorld();
    while(1)
    {
        toggle();
    }
    return (EXIT_SUCCESS);
}

 void initMyPIC18F(void){
    // configure RC0 as a button to change RB0's state
   
     //TRISCbits.TRISC1= 0; //RB0 isan OUTPUT
    
    // configure interrupton PORTB to light ON RE0
    glcd_Init(GLCD_ON); // Init function for GLCF screen
    
    INTCONbits.GIE= 1;
    INTCONbits.RBIE= 1;
    /*
    TRISBbits.TRISB4= 1; // RB4 isan INPUT
    TRISBbits.TRISB5= 1; // RB5 isan INPUT
    TRISBbits.TRISB6= 1; // RB6 isan INPUT
    TRISBbits.TRISB7= 1; // RB7 isan INPUT
    TRISEbits.TRISE0 = 0; // RE0 isan OUTPUT
 */
}
 /*
 void PORTB_Interrupt_ISR(void){
    INTCONbits.RBIF= 0;
    // light on RE0 for 1 sec
    PORTEbits.RE0= 1;
    __delay_ms(100);
    PORTEbits.RE0= 0;
 }

 void __interrupt() irq_handle(void){
    if(INTCONbits.RBIF == 1) {
            PORTB_Interrupt_ISR();
    }
}
*/ 
 void toggle(void)
 {
//    if(PORTCbits.RC0 == 1){ 
//        PORTBbits.RB0 = !PORTBbits.RB0;
        PORTCbits.RC1 = 0;
     //  }
        __delay_ms(1000); // 1000 ms delay
        PORTCbits.RC1 = 1;
        __delay_ms(1000); // 1000 ms delay
 }
 
 void MyHelloWorld(void){
     unsigned char x , y;
     char Table[11] = "Hello World";
     
    // for(int i = 0 ; i < 3 ; i ++){  
     //}
     glcd_text_write(Table, 20, 20);    
     
 }
