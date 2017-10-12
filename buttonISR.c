#include "System.h"
#include "alt_types.h"
#include "buttonISR.h"
#include "altera_avalon_pio_regs.h"

#define KEY1 2
#define KEY2 4
#define KEY3 8
#define Timer_On 0x7
#define Timer_Off 0xB

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void buttonISR(void* context)
#else
void buttonISR(void* context, alt_u32 id)
#endif
{
	alt_u8 timer_toggle;	//Toggling clock on or off

    /* Cast context It is important that this be declared 
     * volatile to avoid unwanted compiler optimization.
     */
	alt_u32* pCount = (alt_u32*)context;

    /* Store the value in the PUSHBUTTONS's edge capture register in *context. */
	alt_u8 button_on = IORD(PUSHBUTTONS_BASE, 3);

    /* Reset the PUSHBUTTONS's edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0);

    /* Act upon the interrupt */
    switch (button_on){

    case KEY1:                                  //KEY1 is the second bit in the edge capture register
    	timer_toggle = IORD(INTERVAL_TIMER_BASE, 1);	//Timer state set to timer_toggle
    	if (timer_toggle&0x8){							//Checks if timer is off
    		timer_toggle = Timer_On;
    		LCD_cursor(0,0);							//Starts LCD
    		LCD_text("Timer Running ..");	//Displays what program is doing
    	} else {										//Checking if timer is on
    		timer_toggle = Timer_Off;
    		LCD_cursor(0,0);
    		LCD_text("Timer Paused .."); 	//Displays what program is doing
    	}
    	IOWR(INTERVAL_TIMER_BASE, 1, timer_toggle);
    	break;

    case KEY2:	                                        //KEY2 is the third bit in the edge capture register
    	*pCount = 0;									//Resets counter
    	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount);	//Displays count
    	LCD_cursor(0,1);
    	LCD_text("Key 2 Pressed ..");	//Displays what key is pressed
    	break;

    case KEY3:	                                        
    	toggleBCDHEX();									//Switches between Hex or BCD
    	LCD_cursor(0,1);
    	LCD_text("Key 3 Pressed ..");	//Displays what key is pressed
    	break;

    default:											//Default case
    	break;

    }



    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}

