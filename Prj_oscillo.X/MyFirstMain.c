/*
 * File:   newmain.c
 * Author: Adrian Chazottes
 *
 * Created on 18 novembre 2016, 10:00
 */

#include "main.h"

// for debug
#include <string.h>
/*
#pragma config FOSC = HS 		//oscillator HS
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF 		//Disable watchDog
#pragma config LVP = OFF 		//Disable low voltage programmig
//#pragma config DEBUG = ON		//Debug ON
*/

//#pragma config FOSC = HS // use for High Speed crystal Resonator

/*
 * DEFINE FOR THE GRID
 */
#define x_start             2
#define x_end               (LCD_WIDTH - 2)
#define y_start             1
#define y_end               (LCD_HEIGHT - 3)

#define GRID_config          0

#define max_value_converter (uint32_t) 1023

uint16_t point_to_print; // global value to print
unsigned char index_glcd;
unsigned char mode = 1;
unsigned char mode_reglage = 0;
float amplitude = 0;


/*
 * Pour zoomer dans la courbe (variation temporel) il faut augmenter ou réduire 
 * le délais.
 * Augmenter le délais va permettre de dézoomer la courbe, mais on verra plus
 * d'écart entre les points: il faut enregistrer les points et les écrires dans 
 * le glcd tout les Nombre_de_point_enregistrer/nombre_de_point_a_afficher.
 * Il faut appliquer cette méthode dès que l'écart entre les points devient
 * trop important (Ce nombre de point peut varier selon la fréquence d'entrée ?)
 * Pour dézoomer il suffit de réduire le délais
 * 
 * Pour varier en amplitude, il suffit de brancher un potentiomètre à la valeur
 * max utilisé pour convertir la source en millivolt.
 * 
 * Pour ajouter un offset il faut faire attention à la valeur maximum et
 * convertir entre 0 et (max - offset) sinon la courbe overflow.
 * 
 */

void main (void)
{
    // call the function that will init the PIC
    
    initMyPIC18F();
    glcd_Init(GLCD_ON);
    
    glcd_text_write("Hello there",5,5);
    __delay_us(1000000);
    init_screen_for_oscilloscope(GRID_config); // 0 = no GRID
//    print_curve(point_to_print);

    while(1)
    {
        __delay_us(20000);
        ADCON0bits.GO = 1; //start converter
    }

}// end of the main function

// make sure to have an empty LAST line in any *.c file (just hit an Enter)!



void print_curve(uint16_t point){
    unsigned char x;
            float y;
    point_to_print = point;
    convert_for_oscilloscope();
    y = y_end - ((unsigned char) point_to_print * amplitude) ;
    // même chose
    if ( y < y_start){
        glcd_text_write((unsigned char*)"Overflow !!!!",5,5);
        return;
    }
    
    x = index_glcd;
    if(mode == 0){
        if(index_glcd <= x_end){
            glcd_PlotPixel( x  , (unsigned char) y   , 0xFF);
            index_glcd++;
        }  
    }else{
        glcd_PlotPixel( x  , (unsigned char) y   , 0xFF);
        index_glcd++;
        if(index_glcd >= x_end){
            init_screen_for_oscilloscope(GRID_config);    
        }
    }        
    
    return;
}


// TEST A FAIRE SUR LA CONVERSION (IF > 1023 et vérifier > 5000) + print
// sur l'écran la valeur à chaque étape.
// convert from [0 : 5000] to [0 : 61]
void convert_for_oscilloscope(void){
    decimal_to_millivolt();
    // convert from millivot to printable value
    uint32_t buffer_temp = 0;
    buffer_temp = (uint32_t) point_to_print * (uint32_t) 61 / 5000 ; 
    point_to_print = (uint16_t) buffer_temp;
    return;
}


// convert from [0 : 1023] to [0 : 5000]
void decimal_to_millivolt(void){
    uint32_t buffer_temp; //avoid overflow
    buffer_temp =  ((uint32_t)point_to_print) *((uint32_t)5000)/1023;
    point_to_print  = (uint16_t) buffer_temp;
    return;
}
void single_mode_manager(void){
    mode = !mode;
    init_screen_for_oscilloscope(GRID_config);        
    return;
}

void ADC_mode(uint16_t receiveADC){
    if(mode_reglage == 1){
        // mode to use potar for amplitude or temporal modification
            mode_reglage = !mode_reglage;
            amplitude = (float) receiveADC * ( float )1.2 / 1023; 
            //set mode for the next acquisition to receive the input signal
            TRISEbits.TRISE1 = 1; //AN6/RE1 INPUT FOR ADC Converter
            ADCON0 = 0b00011001; // Channel AN6 selected and converter enable
            ADCON1 = 0b00001000; // 1000 = DAAAAAAA
            
    }else{
        // mode to received the input in AN6 => PIN RE1
            mode_reglage = !mode_reglage; 
            
            /* set mode for the next acquisition for amplitude and temporal 
             * modification
             */
            TRISAbits.TRISA0 = 1; //AN0 INPUT FOR ADC Converter
            ADCON0 = 0b00000001; // Channel AN0 selected and converter enable
            ADCON1 = 0b00001110; // 1110 = DDDDDDDA
            
            print_curve(receiveADC);
        }
}


void init_screen_for_oscilloscope(char grid ){ // ajout de paramètre grille ON/OFF
    glcd_FillScreen(0);
    print_axis();    
    index_glcd = x_start;
    if(grid){
     print_grid();   
    }
    if(mode==0){
        for(int i = x_start ; i < x_end-5 ; i+=2){
            glcd_PlotPixel(i,63,0xFF); //marks for single mode
        }
    }
    return;
}


void print_axis(void){
    unsigned char x,y;
    
    //print arrow for axis y
    x = x_start;
    y = y_start; 
    glcd_PlotPixel(x-2, y+2, 0xFF);
    glcd_PlotPixel(x-1, y+1, 0xFF);
    glcd_PlotPixel(x+1, y+1, 0xFF);
    glcd_PlotPixel(x+2, y+2, 0xFF);
    
    
    // print axis Y
    x = x_start;
    for(y = y_start ; y < y_end ; y ++){
        glcd_PlotPixel(x, y, 0xFF);
    }
    //print arrow for axis X
    x = x_end;
    y = y_end;
    glcd_PlotPixel(x-2, y-2, 0xFF);
    glcd_PlotPixel(x-1, y-1, 0xFF);
    glcd_PlotPixel(x, y, 0xFF);
    glcd_PlotPixel(x-1, y+1, 0xFF);
    glcd_PlotPixel(x-2, y+2, 0xFF);
    
    // print axis X
    y = y_end;
    for(x = x_start ; x < x_end ; x ++){
        glcd_PlotPixel(x, y, 0xFF);
    }
    
}

void print_grid(void){
    unsigned char x,y;
    
    glcd_PlotPixel(x, y, 0xFF);


}