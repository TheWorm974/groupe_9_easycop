#include "init.h"
#include "main.h"
// --- init the PIC18F device
void initMyPIC18F(void)
{
    TRISA = 0x00;
    PORTA = 0x00;
    
    // set all ports as OUTPUTS
	//TRISB = 0x00;
	TRISC = 0x00;
	TRISD = 0x00;
	TRISE = 0x00;

	// set port by port on "all zeros"
	
	//PORTB = 0x00;
	PORTC = 0x00;
    PORTE = 0x00;	

    PORTD = 0x00;
// make sure to have an empty LAST line in any *.c file (just hit an Enter)!

	
    
    init_myInterrupt();
    return;
}
// make sure to have an empty LAST line in any *.c file (just hit an Enter)!

