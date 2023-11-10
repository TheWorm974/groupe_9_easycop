
#include "main.h"

extern uint16_t point_to_print;
#if(Interrupt_enable == 1)


void init_myInterrupt(void){
    INTCONbits.GIE= 1; // Global Interrupt Enable
    INTCONbits.RBIE= 1; //RB Port change Interrupt Enable
    INTCONbits.PEIE = 1; // interrupt for peripheral
    //RCONbits.IPEN = 1; // priority interrupt enable
    //INTCON2bits.RBIP = 1; // priority for RB interruption
    TRISBbits.TRISB6= 1; // RB7 isan INPUT for switching mode
    TRISBbits.TRISB7= 1; // RB7 isan INPUT for switching mode
    TRISEbits.TRISE0 = 0;
    
    
    //TRISEbits.TRISE0 = 0; // RE0 isan OUTPUT
#if (ADC_Converter == 1)
    init_ADC();
#endif //ADC_Converter == 1
    
    
    return;
}
#if (ADC_Converter == 1)

void init_ADC(void){
    TRISAbits.TRISA0 = 1; //RA5 INPUT FOR ADC Converter
    ADCON0 = 0b00000001; // Channel AN0 selected and converter enable
    ADCON1 = 0b00001110; // 1001 = DDDAAAAA
    //TRISEbits.TRISE1 = 1; //AN6/E1 INPUT FOR ADC Converter
    //ADCON0 = 0b00011001; // Channel AN6 selected and converter enable
    //ADCON1 = 0b00001000; // 1110 = DAAAAAAA
    PIE1bits.ADIE = 1 ;// enable AD interrupt
    PIR1bits.ADIF = 0; //clear flag
    ADCON2bits.ADFM = 1; // right justified
    //IPR1bits.ADIP = 0; // priority bit
}
void ADCON_Interrupt_ISR(void){
    uint16_t buffer_temp = 0;
    PIR1bits.ADIF = 0; //avoid interrupt loop
    //LSB et MSB OU EGAL logic décalé de 8
    buffer_temp = buffer_temp |(uint16_t) (ADRESH << 8) | (uint8_t) ADRESL;
    ADC_mode(buffer_temp);
}
#endif //ADC_Converter == 1


void PORTB_Interrupt_ISR(void){
        if(PORTBbits.RB7 == 1){
            single_mode_manager();
        }
        if(PORTBbits.RB6 ==1 ){
            potar_reglage_mode();
        }
    INTCONbits.RBIF= 0; // clear to avoid infinit loop        
 }

 void __interrupt() high_irq_handle(void){
    
#if (ADC_Converter == 1)
    if( PIR1bits.ADIF == 1){ // does ADC finished to convert ?
        ADCON_Interrupt_ISR();
    }
#endif //ADC_Converter == 1
if(INTCONbits.RBIF == 1) { // Does RB[4,5,6,7] change state ?
    INTCONbits.RBIF= 0; // clear to avoid infinit loop   
        PORTB_Interrupt_ISR();
        
    }
 }
#endif //Interrupt_enable
