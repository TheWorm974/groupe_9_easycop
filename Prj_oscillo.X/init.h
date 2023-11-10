/* 
 * File:   init.h
 * Author: Adrian Chazottes
 *
 * Created on 18 novembre 2016, 10:00
 */


#define Interrupt_enable 1
#define ADC_Converter 1

#if(Interrupt_enable == 1)

void init_myInterrupt(void);
#if (ADC_Converter == 1)
void init_ADC(void);
void ADCON_Interrupt_ISR(void);
#endif //ADC_Converter == 1

void PORTB_Interrupt_ISR(void);

void __interrupt() irq_handle(void);

#endif // Interrupt_enable 

#ifndef INIT_H
#define	INIT_H

// --- microchip define file for your PIC
#include <htc.h>

// declare the function prototypes

// inits the PIC18F device
void initMyPIC18F(void);

#endif	/* INIT_H */

