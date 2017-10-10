#include "io.h"
#include "system.h"
#include "alt_types.h"
#include "timerISR.h"
#include "buttonISR.h"


/*******************************************************************************
 * int main()                                                                  *
 *                                                                             *
 ******************************************************************************/
volatile alt_u8 display_as_bcd;		//variable  to toggle between Hex and bcd
int main(void)
{ 
    /* Setup */
    /* Initialize Variables */
	alt_u32 *pCount;				//Pointer to count variable
	static alt_u32 count = 0;		//Static variable so it never changes
	pCount = &count;				//Assigning the address of count to pCount

	display_as_bcd = 1;				//Initial value
    /* Register ISRs */
	alt_irq_register(INTERVAL_TIMER_IRQ, (void*) pCount, timerISR);		//timer interrupt service routine
	alt_irq_register(PUSHBUTTONS_IRQ, (void*) pCount, buttonISR);		//button interrupt service routine
	IOWR(PUSHBUTTONS_BASE, 2, 0xE);										//push button interrupts
    /* Initialize Timer */
	IOWR(INTERVAL_TIMER_BASE, 2, 0xFFFF&INTERVAL_TIMER_FREQ);


	IOWR(INTERVAL_TIMER_BASE, 3, 0xFFFF&(INTERVAL_TIMER_FREQ >> 16));

	IOWR(INTERVAL_TIMER_BASE, 1, 0x7);									//Enables timer
	/* Loop while processing interrupts */
    while( 1 ) 
    {

    }
    return 0; // should never get here
}
